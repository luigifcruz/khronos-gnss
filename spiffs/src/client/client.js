import React from 'react'
import ReactDOM from 'react-dom'
import { Provider } from 'react-redux'
import { BrowserRouter, Route, Switch, Redirect } from 'react-router-dom';
import { configureStore } from '../redux/store'

import App from '../components/App'
import Dashboard from '../components/Dashboard'
import Clock from '../components/Clock'
import Gnss from '../components/Gnss'
import Settings from '../components/Settings'
import Tuner from '../components/Tuner'

const store = configureStore();

window.onload = () => {
    ReactDOM.render((
        <Provider store={store}>
            <BrowserRouter>
                <App>
                    <Switch>
                        <Route exact path="/" component={Dashboard}/>
                        <Route path="/clock" component={Clock}/>
                        <Route path="/gnss" component={Gnss}/>
                        <Route path="/settings" component={Settings}/>
                        <Route path="/tuner" component={Tuner}/>
                    </Switch>
                </App>
            </BrowserRouter>
        </Provider>
    ), document.getElementById('root'));
};

export default store;
