import React, { Component, PropTypes } from 'react'
import Websocket from 'react-websocket'
import * as rxa from '../redux/actions'
import { connect } from 'react-redux'
import request from 'superagent'
import '../styles/App.scss'

class App extends Component {

	constructor(props) {
      super(props);
    }

    parseStream(payload) {
        if (payload.method == "bulk_update") {
            console.log("[STREAM] New bulk broadcast received.");
        }

        if (payload.method == "delta_update") {
            console.log("[STREAM] New delta broadcast received.");
        }
        
        payload.changes.forEach((value) => {
            if (value.zone == "state") {
                this.props.dispatch(rxa.updateState(value.key, value.value));
            }

            if (value.zone == "settings") {
                this.props.dispatch(rxa.updateSettings(value.key, value.value));
            }
        })
    }

    handleData(data) {
        let payload = JSON.parse(data);
        if (payload.type == "broadcast") {
            this.parseStream(payload)
        }
    }

    handleEvent(data) {
        console.log("[STREAM] Connected to Khronos via WebSocket.");
    }

    render() {
        return (
            <div>
            	<Websocket url='ws://10.10.1.7:8080/api/stream' onOpen= {this.handleEvent.bind(this)} onMessage={this.handleData.bind(this)}/>
                <p>LED Status: {this.props.settings.led_status}</p>
                <p>Serial TX Status: {this.props.settings.serial_tx_active}</p>
                {this.props.children}
            </div>
        )
    }

}

App.propTypes = rxa.props
export default connect(rxa.mapStateToProps)(App)
