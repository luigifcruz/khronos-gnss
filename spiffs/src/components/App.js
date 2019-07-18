import { withRouter } from 'react-router-dom'
import React, { Component } from 'react'
import Websocket from 'react-websocket'
import * as rxa from '../redux/actions'
import { Link } from 'react-router-dom'
import { connect } from 'react-redux'
import { TimeEvent } from 'pondjs'
import request from 'superagent'
import prefix from 'superagent-prefix'

import '../styles/App.scss'

class App extends Component {

    constructor(props) {
        super(props);
        this.state = {
            prefix: "khronos.local"
        }
    }

    componentDidMount() {
        request
            .post('/api/request')
            .send({method: "get", target: "data"})
            .use(prefix(`http://${this.state.prefix}:8080`))
            .end((err, res) => {
                if (err || JSON.parse(res.text).method !== "bulk_update") {
                    console.log("[STREAM] Khronos didn't responded. Connecting to localhost...");
                    this.setState({ prefix: "localhost" });
                };
            });
    }

    parseStream(payload) {        
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
            	<Websocket url={`ws://${this.state.prefix}:8080/api/stream`} onOpen={this.handleEvent.bind(this)} onMessage={this.handleData.bind(this)}/>
                <Link to="/" className="Khronos">Khro9iadsnos</Link>
                {this.props.children}
            </div>
        )
    }

}

App.propTypes = rxa.props
export default withRouter(connect(rxa.mapStateToProps)(App))
