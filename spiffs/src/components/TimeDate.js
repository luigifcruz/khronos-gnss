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
        console.log(this.props.state.latitude, this.props.state.longitude)
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
                    <div className="PredictionBox">
                        <p>Sunrise</p>
                        <h4><Clock format={'HH:mm:ss'} date={this.state.suncalc.sunrise}/></h4>
                        <label>When the top edge of the sun crosses the horizon.</label>
                    </div>
                    <div className="PredictionBox">
                        <p>Sunrise End</p>
                        <h4><Clock format={'HH:mm:ss'} date={this.state.suncalc.sunriseEnd}/></h4>
                        <label>When the entire sun is above the horizon.</label>
                    </div>
                    <div className="PredictionBox">
                        <p>Solar Noon</p>
                        <h4><Clock format={'HH:mm:ss'} date={this.state.suncalc.solarNoon}/></h4>
                        <label>When the sun reaches the higher elevation of the day.</label>
                    </div>
                    <div className="PredictionBox">
                        <p>Golden Hour End</p>
                        <h4><Clock format={'HH:mm:ss'} date={this.state.suncalc.goldenHour}/></h4>
                        <label>When the best time for photography ends.</label>
                    </div>
                    <div className="PredictionBox">
                        <p>Sunset Start</p>
                        <h4><Clock format={'HH:mm:ss'} date={this.state.suncalc.sunsetStart}/></h4>
                        <label>When the sun touches the horizon.</label>
                    </div>
                    <div className="PredictionBox">
                        <p>Sunset</p>
                        <h4><Clock format={'HH:mm:ss'} date={this.state.suncalc.sunset}/></h4>
                        <label>When the sun disappears below the horizon.</label>
                    </div>
                    <div className="PredictionBox">
                        <p>Dusk</p>
                        <h4><Clock format={'HH:mm:ss'} date={this.state.suncalc.dusk}/></h4>
                        <label>When the nautical twilight starts.</label>
                    </div>
                    <div className="PredictionBox">
                        <p>Nautical Dusk</p>
                        <h4><Clock format={'HH:mm:ss'} date={this.state.suncalc.nauticalDusk}/></h4>
                        <label>When the evening astronomical twilight starts.</label>
                    </div>
                    <div className="PredictionBox">
                        <p>Night Start</p>
                        <h4><Clock format={'HH:mm:ss'} date={this.state.suncalc.night}/></h4>
                        <label>When it's dark enough for astronomical observations.</label>
                    </div>
                    <div className="PredictionBox">
                        <p>Nadir</p>
                        <h4><Clock format={'HH:mm:ss'} date={this.state.suncalc.nadir}/></h4>
                        <label>When the sun reaches the lowest position. Darkest time of the night!</label>
                    </div>
                    <div className="PredictionBox">
                        <p>Night End</p>
                        <h4><Clock format={'HH:mm:ss'} date={this.state.suncalc.nightEnd}/></h4>
                        <label>When the morning astronomical twilight starts.</label>
                    </div>
                    <div className="PredictionBox">
                        <p>Dawn</p>
                        <h4><Clock format={'HH:mm:ss'} date={this.state.suncalc.dawn}/></h4>
                        <label>When the morning civil twilight starts.</label>
                    </div>
                <h3>Lunar Time</h3>
            </div>
        )
    }

}

TimeDate.propTypes = rxa.props
export default connect(rxa.mapStateToProps)(TimeDate)  