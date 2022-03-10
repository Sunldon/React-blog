import App from 'next/app'
import 'antd/dist/antd.css'
import '../static/style/pages/comm.css'
import '../static/style/components/header.css'
import '../static/style/pages/index.css'
import '../static/style/components/author.css'
import '../static/style/components/advert.css'
import '../static/style/components/footer.css'
import '../static/style/pages/detailed.css'
import 'highlight.js/styles/monokai-sublime.css';

// export default App
export default function MyApp({ Component, pageProps }) {
  return <Component {...pageProps} />
}