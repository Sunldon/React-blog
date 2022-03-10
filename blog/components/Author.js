import { Avatar, Divider } from 'antd'

const Author = () => {
  return (
    <div className="author-div comm-box">
      <div> <Avatar size={100} src="https://s1.ax1x.com/2022/03/09/bfK8bQ.png" /></div>
      <div className="author-introduction">
        ctrl+c, ctrl+v
        <Divider>社交账号</Divider>
        <Avatar size={28} icon="github" className="account" />
        <Avatar size={28} icon="qq" className="account" />
        <Avatar size={28} icon="wechat" className="account" />
      </div>
    </div>
  )
}

export default Author