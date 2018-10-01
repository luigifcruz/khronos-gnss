import React, { Component } from 'react'
import * as rxa from '../redux/actions'
import { connect } from 'react-redux'

import Variables from './modules/GnssVariables'
import Maps from './modules/GnssMaps'
import Chart from './modules/GnssChart'
import Orbits from './modules/GnssOrbits'

import '../styles/Gnss.scss'

class Gnss extends Component {

    render() {
        return (
            <div className="Gnss">
                <h2>GNSS Navigation</h2>
                <Variables state={this.props.state} />
                <Orbits state={this.props.state} />
            </div>
        )
    }

}

Gnss.propTypes = rxa.props
export default connect(rxa.mapStateToProps)(Gnss)  
