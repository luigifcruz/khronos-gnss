import React, { Component, PropTypes } from 'react'
import * as rxa from '../redux/actions'
import { connect } from 'react-redux'
import '../styles/Gnss.scss'

class Gnss extends Component {

    constructor(props) {
      super(props);
    }

    render() {
        return (
            <div>
                <h3>GNSS Dashboard</h3>
                <p>Latitude: {this.props.state.latitude}</p>
                <p>Longitude: {this.props.state.longitude}</p>
                <p>True North (d): {this.props.state.true_north}</p>
                <p>Ground Speed (kph): {this.props.state.ground_speed}</p>
                <p>Altitude (m): {this.props.state.altitude}</p>
                <hr></hr>
                <p>GNSS Fix Quality: {this.props.state.gnss_fix_quality}</p>
                <p>GNSS Fix Type: {this.props.state.gnss_fix_type}</p>
                <p>GPS Satellites Count: {this.props.state.sat_count_gps}</p>
                <p>GLONASS Satellites Count: {this.props.state.sat_count_glonass}</p>
            </div>
        )
    }

}

Gnss.propTypes = rxa.props
export default connect(rxa.mapStateToProps)(Gnss)  
