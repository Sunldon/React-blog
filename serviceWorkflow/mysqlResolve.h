
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include "workflow/HttpUtil.h"
#include "workflow/WFFacilities.h"
#include "workflow/MySQLResult.h"
#include "json/value.h"
using namespace protocol;

bool mysqlResolve(WFMySQLTask *task, Json::Value& data)
{
  if (task->get_state() != WFT_STATE_SUCCESS)
  {
    fprintf(stderr, "error msg: %s\n",
            WFGlobal::get_error_string(task->get_state(),
                                       task->get_error()));                                 
    return false;
  }
  
  MySQLResponse *sqlresp = task->get_resp();
  MySQLResultCursor cursor(sqlresp);
  const MySQLField *const *fields;
  std::vector<MySQLCell> arr;

  do
  {
    if (cursor.get_cursor_status() != MYSQL_STATUS_GET_RESULT &&
        cursor.get_cursor_status() != MYSQL_STATUS_OK)
    {
      break;
    }

    if (cursor.get_cursor_status() == MYSQL_STATUS_GET_RESULT)
    {
      // nocopy api
      fields = cursor.fetch_fields();      
      while (cursor.fetch_row(arr))
      {
        Json::Value object;
        for (size_t i = 0; i < arr.size(); i++)
        {          
          if (arr[i].is_string())
          {
            std::string res = arr[i].as_string();
            object[fields[i]->get_name().c_str()] = arr[i].as_string();
          }
          else if (arr[i].is_int())
          {
            object[fields[i]->get_name().c_str()] = arr[i].as_int();
          }
          else if (arr[i].is_ulonglong())
          {
            object[fields[i]->get_name().c_str()] = (uint64_t)arr[i].as_ulonglong();
          }
          else if (arr[i].is_float())
          {
            object[fields[i]->get_name().c_str()] = arr[i].as_float();
          }
          else if (arr[i].is_double())
          {
            object[fields[i]->get_name().c_str()] = arr[i].as_double();
          }
          else if (arr[i].is_date())
          {
            object[fields[i]->get_name().c_str()] = arr[i].as_string();
          }
          else if (arr[i].is_time())
          {
            object[fields[i]->get_name().c_str()] = arr[i].as_string();
          }
          else if (arr[i].is_datetime())
          {
            object[fields[i]->get_name().c_str()] = arr[i].as_string();
          }
          else if (arr[i].is_null())
          {
            object[fields[i]->get_name().c_str()] = "";
          }
          else
          {
						std::string res = arr[i].as_binary_string();
            object[fields[i]->get_name().c_str()] = res;       
          }
        }
        data.append(object);
      }
    }
    else if (cursor.get_cursor_status() == MYSQL_STATUS_OK)
    {
      fprintf(stderr, "  OK. %llu ", cursor.get_affected_rows());
      if (cursor.get_affected_rows() == 1)
      {
        fprintf(stderr, "row ");
      // isSuccess: insertSuccess,
      // insertId: insertId
        data["isSuccess"] = true;
        data["insertId"] = (uint64_t)cursor.get_insert_id();
      }
      else
      {
        fprintf(stderr, "rows ");
      }
      fprintf(stderr, "affected. %d warnings. insert_id=%llu. %s\n",
              cursor.get_warnings(), cursor.get_insert_id(),
              cursor.get_info().c_str());
    }
  } while (cursor.next_result_set());

  if (sqlresp->get_packet_type() == MYSQL_PACKET_ERROR)
  {
    fprintf(stderr, "ERROR. error_code=%d %s\n",
            task->get_resp()->get_error_code(),
            task->get_resp()->get_error_msg().c_str());
  }
  else if (sqlresp->get_packet_type() == MYSQL_PACKET_OK) // just check origin APIs
  {
    fprintf(stderr, "OK. %llu ", task->get_resp()->get_affected_rows());
    if (task->get_resp()->get_affected_rows() == 1)
      fprintf(stderr, "row ");
    else
      fprintf(stderr, "rows ");
    fprintf(stderr, "affected. %d warnings. insert_id=%llu. %s\n",
            task->get_resp()->get_warnings(),
            task->get_resp()->get_last_insert_id(),
            task->get_resp()->get_info().c_str());
  }
  return true;
}