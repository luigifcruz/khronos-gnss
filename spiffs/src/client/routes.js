import React from 'react'
import { render } from 'react-dom'
import { Router, Route, Redirect, IndexRedirect, IndexRoute } from 'react-router'

import App from '../components/App'
import Dashboard from '../components/Dashboard'
import Clock from '../components/Clock'
import Gnss from '../components/Gnss'
import Settings from '../components/Settings'
import Tuner from '../components/Tuner'

const routes = (
  	<Route path="/" component={App}>
	    <IndexRoute component={Dashboard}/>
	    <Route path="clock" component={Clock}/>
	    <Route path="gnss" component={Gnss}/>
	    <Route path="settings" component={Settings}/>
	    <Route path="tuner" component={Tuner}/>
  	</Route>
);

export default routes;