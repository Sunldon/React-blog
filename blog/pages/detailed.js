import React, { useState } from 'react'
import Head from 'next/head'
import { Row, Col, Affix, Breadcrumb } from 'antd'
import { FireOutlined, CalendarOutlined, FolderOutlined } from '@ant-design/icons';
import Header from '../components/Header'
import Author from '../components/Author'
import Advert from '../components/Advert'
import Footer from '../components/Footer'
import 'markdown-navbar/dist/navbar.css';
import axios from 'axios'
import { marked } from 'marked';
import 'highlight.js/styles/monokai-sublime.css';
import Tocify from '../components/tocify.tsx'
import servicePath from '../config/apiUrl'

const Detailed = (props) => {
  const tocify = new Tocify()
  const renderer = new marked.Renderer();
  renderer.heading = function (text, level, raw) {
    const anchor = tocify.add(text, level);
    return `<a id="${anchor}" href="#${anchor}" class="anchor-fix"><h${level}>${text}</h${level}></a>\n`;
  };

  marked.setOptions({
    renderer: renderer,
    gfm: true,
    pedantic: false,
    sanitize: false,
    tables: true,
    breaks: false,
    smartLists: true,
    smartypants: false
    // highlight: function (code) {
    //   return hljs.highlightAuto(code).value;
    // }
  });
  let html = marked(props.article_content)

  return (
    <>
      <Head>
        <title>博客详细页</title>
      </Head>
      <Header />
      <Row className="comm-main" type="flex" justify="center">
        <Col className="comm-left" xs={24} sm={24} md={16} lg={18} xl={14}  >
          <div>
            <div className="bread-div">
              <Breadcrumb>
                <Breadcrumb.Item><a href="/">首页</a></Breadcrumb.Item>
                <Breadcrumb.Item>{props.typeName}</Breadcrumb.Item>
                <Breadcrumb.Item> {props.title}</Breadcrumb.Item>
              </Breadcrumb>
            </div>

            <div>
              <div className="detailed-title">
                {props.title}
              </div>

              <div className="list-icon center">
                <span><CalendarOutlined /> {props.addTime}</span>
                <span><FolderOutlined /> {props.typeName}</span>
                <span><FireOutlined /> {props.view_count}人</span>
              </div>

              <div className="detailed-content"
                dangerouslySetInnerHTML={{ __html: html }}   >
              </div>
            </div>
          </div>
        </Col>

        <Col className="comm-right" xs={0} sm={0} md={7} lg={5} xl={4}>
          <Author />
          <Advert />
          <Affix offsetTop={5}>
            <div className="detailed-nav comm-box">
              <div className="nav-title">文章目录</div>
              <div className="toc-list">
                {tocify && tocify.render()}
              </div>
            </div>
          </Affix>
        </Col>
      </Row>
      <Footer />
    </>
  )

}

Detailed.getInitialProps = async (context) => {
  console.log('context.query', context.query)
  let id = context.query.id
  const promise = new Promise((resolve) => {
    axios(servicePath.getArticleById + id).then(
      (res) => {
        //console.log("Detailed.getInitialProps", res.data)
        resolve(res.data.data[0])
      }
    )
  })
  return await promise
}

export default Detailed