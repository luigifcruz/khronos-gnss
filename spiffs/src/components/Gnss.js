import React, { Component } from 'react'
import * as rxa from '../redux/actions'
import { connect } from 'react-redux'

import Variables from './modules/GnssVariables'
import Maps from './modules/GnssMaps'
import Chart from './modules/GnssChart'

import '../styles/Gnss.scss'

class Gnss extends Component {

    render() {
        return (
            <div className="Gnss">
                <h2>GNSS Navigation</h2>
                <Maps state={this.props.state} mapdata={this.props.mapdata} />
                <Variables state={this.props.state} />
                <Chart history={this.props.history} />
            </div>
        )
    }

}

Gnss.propTypes = rxa.props
export default connect(rxa.mapStateToProps)(Gnss)  
