import React, { useState, useEffect } from 'react';
import 'antd/dist/antd.min.css';
import '../static/css/Login.css';
import { Card, Input, Button, Spin, message } from 'antd';
import axios from 'axios'
import servicePath from '../config/apiUrl'
import { UserOutlined, KeyOutlined } from '@ant-design/icons';
import { useNavigate } from "react-router-dom";

function Login() {
  const [userName, setUserName] = useState('')
  const [password, setPassword] = useState('')
  const [isLoading, setIsLoading] = useState(false)
  let navigate = useNavigate();
  useEffect(() => { return () => {clearTimeout() } }, [])

  const checkLogin = () => {
    setIsLoading(true)
    if (!userName) {
      message.error('用户名不能为空')
      return false
    } else if (!password) {
      message.error('密码不能为空')
      return false
    }
    let dataProps = {
      'userName': userName,
      'password': password
    }
    axios({
      method: 'post',
      url: servicePath.checkLogin,
      header: { 'Access-Control-Allow-Origin': '*' },
      data: dataProps,
      withCredentials: false
    }).then(
      res => {
        setIsLoading(false)
        //console.log("login ", res.data)
        if (res.data.data[0]['userName'] === userName) {
          localStorage.setItem('openId', res.data.openId)
          navigate('/index')
        } else {
          message.error('用户名密码错误')
        }
      }
    ).catch(function (error) {
      setIsLoading(false)
      console.log('checkLogin', error.toJSON());
    })

    setTimeout(() => {
      setIsLoading(false)
    }, 1000)
  }

  return (
    <div className="login-div">
      <Spin tip="Loading..." spinning={isLoading}>
        <Card title="Sun Blog System" bordered={true} style={{ width: 400 }} >
          <Input
            id="userName"
            size="large"
            placeholder="Enter your userName"
            prefix={<UserOutlined type="user" style={{ color: 'rgba(0,0,0,.25)' }} />}
            onChange={(e) => { setUserName(e.target.value) }}
          />
          <br /><br />
          <Input.Password
            id="password"
            size="large"
            placeholder="Enter your password"
            prefix={<KeyOutlined type="key" style={{ color: 'rgba(0,0,0,.25)' }} />}
            onChange={(e) => { setPassword(e.target.value) }}
          />
          <br /><br />
          <Button type="primary" size="large" block onClick={checkLogin} > Login in </Button>
        </Card>
      </Spin>
    </div>
  )
}

export default Login