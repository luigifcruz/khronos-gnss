import React from 'react';
import ReactDOM from 'react-dom';
import { Provider } from 'react-redux';
import { Router, browserHistory } from 'react-router';
import { configureStore } from '../redux/store';
import routes from './routes';

const store = configureStore();

window.onload = () => {
    ReactDOM.render((
        <Provider store={store}>
            <Router history={browserHistory} routes={routes}/>
        </Provider>
    ), document.getElementById('root'));
};

export default store;
