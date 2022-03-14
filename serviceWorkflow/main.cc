/*
  Copyright (c) 2019 Sogou, Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  Author: Xie Han (xiehan@sogou-inc.com;63350856@qq.com)
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include "workflow/HttpMessage.h"
#include "workflow/HttpUtil.h"
#include "workflow/WFServer.h"
#include "workflow/WFHttpServer.h"
#include "workflow/WFFacilities.h"
#include "workflow/MySQLResult.h"
#include "json/value.h"
#include "json/json.h"
#include "mysqlResolve.h"
#include <sstream>

static int count = 1;
using namespace protocol;

void mysql_callback(WFMySQLTask *task)
{
  protocol::HttpResponse *resp = (protocol::HttpResponse *)task->user_data;
  Json::Value data;
  if (mysqlResolve(task, data["data"]))
  {
    std::string output = Json::FastWriter().write(data);
    // printf("output %s \n", output.c_str());
    resp->append_output_body(output);
    resp->set_status_code("200");
    resp->set_reason_phrase("OK");
  }
  else
  {
    resp->set_status_code("503");
    resp->append_output_body("<html>503 Internal Server Error.</html>");
  }
}

void process(WFHttpTask *server_task)
{
  protocol::HttpRequest *req = server_task->get_req();
  protocol::HttpResponse *resp = server_task->get_resp();
  // long long seq = server_task->get_task_seq();
  // protocol::HttpHeaderCursor cursor(req);
  std::string name;
  std::string value;
  // char buf[8192];
  // int len;

  std::string requestUrl;
  req->get_request_uri(requestUrl);
  printf("requestUrl:%s %d \n", requestUrl.c_str(), count++);

  resp->set_http_version("HTTP/1.1");
  resp->add_header_pair("Access-Control-Allow-Origin", "*");
  resp->add_header_pair("Access-Control-Allow-Credentials", "true");
  resp->add_header_pair("Access-Control-Allow-Methods", "*");
  resp->add_header_pair("Access-Control-Allow-Headers", "Content-Type, Access-Token, x-dataplus-csrf,");
  resp->add_header_pair("Access-Control-Expose-Headers", "*");
  resp->add_header_pair("Content-Type", "application/json;text/html;charset=utf-8'");
  resp->add_header_pair("Server", "Sun Server");

  if (requestUrl.find("getArticleList") != std::string::npos)
  {
    std::string msyqlUrl = "mysql://root:123456@172.20.208.1:3306/react_blog";
    WFMySQLTask *task = WFTaskFactory::create_mysql_task(msyqlUrl, 0, mysql_callback);
    const std::string query = "SELECT article.id as id, article.title as title, article.introduce as introduce, \
      FROM_UNIXTIME(article.addTime, \'%Y-%m-%d\' ) as addTime,\
      article.view_count as view_count, \
      type.typeName as typeName \
      FROM article LEFT JOIN type ON article.type_id = type.Id \
      ORDER BY article.id DESC ";
    task->get_req()->set_query(query.c_str());
    task->user_data = resp; 
    series_of(server_task)->push_back(task);
  }
  else if (requestUrl.find("getArticleById") != std::string::npos)
  {
    std::string msyqlUrl = "mysql://root:123456@172.20.208.1:3306/react_blog";
    WFMySQLTask *task = WFTaskFactory::create_mysql_task(msyqlUrl, 0, mysql_callback);
    std::string id = requestUrl.substr(requestUrl.find_last_of('/') + 1, requestUrl.size() - requestUrl.find_last_of('/') + 1);
    const std::string query = "SELECT article.id as id, \
      article.title as title,\
      article.introduce as introduce,\
      article.article_content as article_content,\
      FROM_UNIXTIME(article.addTime,\'%Y-%m-%d\' ) as addTime,\
      article.view_count as view_count ,\
      type.typeName as typeName ,\
      type.id as typeId \
      FROM article LEFT JOIN type ON article.type_id = type.id \
      WHERE article.id=" + id;

    // printf("id=%s \n", id.c_str());
    task->get_req()->set_query(query.c_str());
    task->user_data = resp; 
    series_of(server_task)->push_back(task);
  }
  else if (requestUrl.find("getTypeInfo") != std::string::npos)
  {
    std::string msyqlUrl = "mysql://root:123456@172.20.208.1:3306/react_blog";
    WFMySQLTask *task = WFTaskFactory::create_mysql_task(msyqlUrl, 0, mysql_callback);
    const std::string query = "SELECT * FROM type";
    task->get_req()->set_query(query.c_str());
    task->user_data = resp; 
    series_of(server_task)->push_back(task);
  }
  else if (requestUrl.find("checkLogin") != std::string::npos)
  {
    const void *body;
    size_t body_len;
    req->get_parsed_body(&body, &body_len);
    std::string stringJson((char *)body);
    Json::Reader reader;
    Json::Value value;
    reader.parse(stringJson, value);
    std::string msyqlUrl = "mysql://root:123456@172.20.208.1:3306/react_blog";
    WFMySQLTask *task = WFTaskFactory::create_mysql_task(msyqlUrl, 0, mysql_callback);
    const std::string query = " SELECT userName FROM admin_user WHERE userName = '" + value["userName"].asString() + "' AND password = '" + value["password"].asString() + "'";
    // printf("query=%s \n", query.c_str());
    task->get_req()->set_query(query.c_str());
    task->user_data = resp; 
    series_of(server_task)->push_back(task);
  }
  else if (requestUrl.find("addArticle") != std::string::npos)
  {
    const void *body;
    size_t body_len;
    req->get_parsed_body(&body, &body_len);
    std::string stringJson((char *)body);
    Json::Reader reader;
    Json::Value value;
    reader.parse(stringJson, value);    
    std::string msyqlUrl = "mysql://root:123456@172.20.208.1:3306/react_blog";
    WFMySQLTask *task = WFTaskFactory::create_mysql_task(msyqlUrl, 0, mysql_callback);

    std::string query;
    query += "INSERT INTO article(type_id, title, article_content, introduce, addTime, view_count, article_content_html, introduce_html)";
    query += "VALUES(";
    const std::vector<std::string> fields{"type_id", "title", "article_content", "introduce", "addTime", "view_count", "article_content_html", "introduce_html"};
    for (const auto& field : fields)
    {
      query += "'";
      query += value[field].asString();
      query += "',";
    }
    query.pop_back();
    query += ")";
    // printf("query=%s \n", query.c_str());
    task->get_req()->set_query(query.c_str());
    task->user_data = resp; 
    series_of(server_task)->push_back(task);    
  }
  else if (requestUrl.find("updateArticle") != std::string::npos)
  {
    const void *body;
    size_t body_len;
    req->get_parsed_body(&body, &body_len);
    std::string stringJson((char *)body);
    Json::Reader reader;
    Json::Value value;
    reader.parse(stringJson, value);    
    std::string msyqlUrl = "mysql://root:123456@172.20.208.1:3306/react_blog";
    WFMySQLTask *task = WFTaskFactory::create_mysql_task(msyqlUrl, 0, mysql_callback);

    std::string query;
    query += "UPDATE article SET ";
    const std::vector<std::string> fields{"type_id", "title", "article_content", "introduce", "addTime", "article_content_html", "introduce_html"};
    for (const auto& field : fields)
    {
      query += field;
      query += "='";
      query += value[field].asString();
      query += "',";
    }
    query.pop_back();
    query += "WHERE id = '" + value["id"].asString() + "'";
    // printf("query=%s \n", query.c_str());
    task->get_req()->set_query(query.c_str());
    task->user_data = resp; 
    series_of(server_task)->push_back(task);    
  }
  else if (requestUrl.find("delArticle") != std::string::npos)
  {
    std::string id = requestUrl.substr(requestUrl.find_last_of('/') + 1, requestUrl.size() - requestUrl.find_last_of('/') + 1);
    std::string msyqlUrl = "mysql://root:123456@172.20.208.1:3306/react_blog";
    WFMySQLTask *task = WFTaskFactory::create_mysql_task(msyqlUrl, 0, mysql_callback);
    std::string query = "DELETE FROM article WHERE id = '" + id + "'";
    task->get_req()->set_query(query.c_str());
    task->user_data = resp; 
    series_of(server_task)->push_back(task);    
  }
}

static WFFacilities::WaitGroup wait_group(1);

void sig_handler(int signo)
{
  wait_group.done();
}

int main(int argc, char *argv[])
{
  unsigned short port;
  signal(SIGINT, sig_handler);

  WFHttpServer server(process);
  port = 7001;
  if (server.start(port) == 0)
  {
    wait_group.wait();
    server.stop();
  }
  else
  {
    perror("Cannot start server");
    exit(1);
  }

  return 0;
}
