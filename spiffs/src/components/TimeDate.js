import { CopyToClipboard } from 'react-copy-to-clipboard'
import { Clock as Analog } from 'react-clock'
import React, { Component } from 'react'
import * as rxa from '../redux/actions'
import { connect } from 'react-redux'
import Clock from 'react-live-clock'
import SunCalc from 'suncalc'
import '../styles/TimeDate.scss'

class TimeDate extends Component {

    constructor(props) {
        super(props);
        this.state = {
            date: new Date(),
            suncalc: SunCalc.getTimes(new Date(), props.state.latitude, props.state.longitude)
        }
    }

    componentDidMount() {
        setInterval(() => this.setState({ date: new Date() }), 1000);
    }

    render() {
        return (
            <div className="TimeDate">
                <h2>Time & Date</h2>
                <div className="ClockSection">
                    <center>
                        <div className="ClockBlock">
                            <h3>Local Time</h3>
                            <Analog
                                size={250}
                                secondHandWidth={7}
                                secondHandLength={80}
                                minuteHandWidth={10}
                                minuteMarksWidth={2}
                                hourMarksWidth={4}
                                hourHandWidth={10}
                                value={new Date()} />
                            <CopyToClipboard text={new Date()}>
                                <div className="DigitalBlock">
                                    <h4><Clock format={'HH:mm:ss'} ticking={true} /></h4>
                                    <h5><Clock format={'dddd, MMMM Do, YYYY'} /></h5>
                                </div>
                            </CopyToClipboard>
                        </div>
                        <div className="ClockBlock">
                            <h3>Zulu Time</h3>
                            <Analog
                                size={250}
                                secondHandWidth={7}
                                secondHandLength={80}
                                minuteHandWidth={10}
                                minuteMarksWidth={2}
                                hourMarksWidth={4}
                                hourHandWidth={10}
                                value={this.state.date} />
                            <CopyToClipboard text={new Date()}>
                                <div className="DigitalBlock">
                                    <h4><Clock format={'HH:mm:ss'} ticking={true} timezone={'GMT'} /></h4>
                                    <h5><Clock format={'dddd, MMMM Do, YYYY'} timezone={'GMT'} /></h5>
                                </div>
                            </CopyToClipboard>
                        </div>
                        <div className="ClockBlock">
                            <h3>EST Time</h3>
                            <Analog
                                size={250}
                                secondHandWidth={7}
                                secondHandLength={80}
                                minuteHandWidth={10}
                                minuteMarksWidth={2}
                                hourMarksWidth={4}
                                hourHandWidth={10}
                                value={this.state.date} />
                            <CopyToClipboard text={new Date()}>
                                <div className="DigitalBlock">
                                    <h4><Clock format={'HH:mm:ss'} ticking={true} timezone={'US/Eastern'} /></h4>
                                    <h5><Clock format={'dddd, MMMM Do, YYYY'} timezone={'US/Eastern'} /></h5>
                                </div>
                            </CopyToClipboard>
                        </div>
                    </center>
                </div>
                <h3>Solar Time</h3>
                    <p>Sunrise</p><h4>{this.state.suncalc.sunrise.toString()}</h4><label>When the top edge of the sun crosses the horizon.</label>
                    <p>Sunrise End</p><h4>{this.state.suncalc.sunriseEnd.toString()}</h4><label>When the entire sun is above the horizon.</label>
                    <p>Solar Noon</p><h4>{this.state.suncalc.solarNoon.toString()}</h4><label>When the sun reaches the higher elevation of the day.</label>
                    <p>Golden Hour End</p><h4>{this.state.suncalc.goldenHour.toString()}</h4><label>When the best time for photography ends.</label>
                    <p>Sunset Start</p><h4>{this.state.suncalc.sunsetStart.toString()}</h4><label>When the sun touches the horizon.</label>
                    <p>Sunset</p><h4>{this.state.suncalc.sunset.toString()}</h4><label>When the sun disappears below the horizon. Civil twilight starts.</label>
                    <p>Dusk</p><h4>{this.state.suncalc.dusk.toString()}</h4><label>When the nautical twilight starts.</label>
                    <p>Nautical Dusk</p><h4>{this.state.suncalc.nauticalDusk.toString()}</h4><label>When the evening astronomical twilight starts.</label>
                    <p>Night Start</p><h4>{this.state.suncalc.night.toString()}</h4><label>When it's dark enough for astronomical observations.</label>
                    <p>Nadir</p><h4>{this.state.suncalc.nadir.toString()}</h4><label>When the sun reaches the lowest position. Darkest time of the night!</label>
                    <p>Night End</p><h4>{this.state.suncalc.nightEnd.toString()}</h4><label>When the morning astronomical twilight starts.</label>
                    <p>Nautical Dawn</p><h4>{this.state.suncalc.nauticalDawn.toString()}</h4><label>When the morning nautical twilight starts.</label>
                    <p>Dawn</p><h4>{this.state.suncalc.dawn.toString()}</h4><label>When the morning twilight ends and morning civil twilight starts.</label>
                <h3>Lunar Time</h3>
            </div>
        )
    }

}

TimeDate.propTypes = rxa.props
export default connect(rxa.mapStateToProps)(TimeDate)  