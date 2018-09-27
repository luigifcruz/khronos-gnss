import { withRouter } from 'react-router-dom'
import React, { Component } from 'react'
import Websocket from 'react-websocket'
import * as rxa from '../redux/actions'
import { Link } from 'react-router-dom'
import { connect } from 'react-redux'
import { TimeEvent } from 'pondjs'
import request from 'superagent'

import '../styles/App.scss'

class App extends Component {

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
            this.updateState(payload.timestamp)
        }
    }

    updateState(timestamp) {
        this.props.dispatch(rxa.updateHistory(new TimeEvent(new Date(Number(timestamp)/1000), this.props.state)));

        let { latitude, longitude } = this.props.state;
        this.props.dispatch(rxa.updateMapdata({
            location: new google.maps.LatLng(latitude, longitude), 
            weight: 1
        }));
    }

    handleEvent(data) {
        console.log("[STREAM] Connected to Khronos via WebSocket.");
    }

    render() {
        return (
            <div className="App">
            	<Websocket url='ws://khronos.local:8080/api/stream' onOpen={this.handleEvent.bind(this)} onMessage={this.handleData.bind(this)}/>
                <Link to="/" className="Khronos">Khronos</Link>
                {this.props.children}
            </div>
        )
    }

}

App.propTypes = rxa.props
export default withRouter(connect(rxa.mapStateToProps)(App))
