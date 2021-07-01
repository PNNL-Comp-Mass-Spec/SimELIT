// import React from 'react';
// import ReactDOM from 'react-dom';
// import './index.css';
// import App from './App';
// import * as serviceWorker from './serviceWorker';

// ReactDOM.render(
//   <React.StrictMode>
//     <App />
//   </React.StrictMode>,
//   document.getElementById('root')
// );

// // If you want your app to work offline and load faster, you can change
// // unregister() to register() below. Note this comes with some pitfalls.
// // Learn more about service workers: https://bit.ly/CRA-PWA
// serviceWorker.unregister();
import React from "react";
import ReactDOM from "react-dom";
// import { createBrowserHistory } from "history";
// import { Router, Route, Switch, Redirect } from "react-router-dom";

// import data from './data';
// import FileTreeView from "./views/FileTreeView"

// import "bootstrap/dist/css/bootstrap.css";
// import "./assets/scss/paper-dashboard.scss?v=1.1.0";
// import "./assets/css/paper-dashboard.min.css";
// import "./assets/demo/demo.css";
// import "perfect-scrollbar/css/perfect-scrollbar.css";


import Layout from "./Layout.js";

// const hist = createBrowserHistory();

ReactDOM.render(
    <Layout />,
    document.getElementById("root")
);
