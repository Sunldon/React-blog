import React from 'react';
import {
  BrowserRouter,
  Routes,
  Route,
  Outlet
} from "react-router-dom";
import Login from './Login'
import AdminIndex from './AdminIndex';
import AddArticle from './AddArticle'
import ArticleList from './ArticleList';

function Main() {
  return (
    <BrowserRouter>
      <Routes>
        <Route path="/" element={<Login />} />      
        <Route path="index" element={<AdminIndex />} >
          <Route path="addArticle" element={<AddArticle />} /> 
          <Route path="addArticle/:id" element={<AddArticle />} /> 
          <Route path="articleList" element={<ArticleList />} />
        </Route>
        <Route
          path="*"
          element={
            <main style={{ padding: "1rem" }}>
              <p>There's nothing here!</p>
            </main>
          }
        />
      </Routes>
    </BrowserRouter>
  )
}
export default Main