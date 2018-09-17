import { createStore, applyMiddleware } from 'redux'
import Reducer from './reducer'
import createLogger from 'redux-logger'

let defaultState = {
    'state': {
    	'gps_fix_quality': 0,
    	'gps_fix_type': 0,
    	'gps_sat_numb': 0,
    	'glonass_sat_numb': 0
    },
    'settings': {
    	'ws_update_rate': 0,
    	'led_status': 0,
    	'serial_tx_active': 0
    }
}

const middleware = [ ]

if (process.env.NODE_ENV == 'development') {
    middleware.push(createLogger())
}

function configureStore(initialState = defaultState) {
    const store = createStore(Reducer, initialState, applyMiddleware(...middleware));
    return store;
}

export { configureStore }
