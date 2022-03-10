import React from 'react'
import Router from 'next/router'
import Link from 'next/link'
import { Row, Col, Menu } from 'antd'
import {HomeOutlined, YoutubeOutlined, SmileOutlined} from '@ant-design/icons';

const Header = () => {
  //跳转到列表页
  const handleClick = (e) => {
    // console.log('header ', e.key)
    if (e.key == 0) {
      Router.push('/')
    } else {
      Router.push('/list?id=' + e.key)
    }
  }

  return (
    <div className="header">
      <Row type="flex" justify="center">
        <Col xs={24} sm={24} md={10} lg={13} xl={11}>
          <span className="header-logo">
            <Link href={{ pathname: '/index' }}>
              <a> Sun</a>
            </Link>
          </span>
          <span className="header-txt">醉后不知天在谁，满船星梦压星河</span>
        </Col>

        <Col className="memu-div" xs={0} sm={0} md={14} lg={10} xl={7}>
          <Menu
            mode="horizontal"
            onClick={handleClick}
          >
          <Menu.Item key="0">
            <HomeOutlined />
            首页
          </Menu.Item>
          <Menu.Item key="1">
            <YoutubeOutlined />
            视频
          </Menu.Item>
          <Menu.Item key="2">
            <SmileOutlined />
            生活
          </Menu.Item>
          </Menu>
        </Col>
      </Row>
    </div>
  )
}

export default Header