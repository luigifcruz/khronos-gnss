import React, { Component, PropTypes } from 'react'
import * as rxa from '../redux/actions'
import { connect } from 'react-redux'
import '../styles/Gnss.scss'
import Ring from "ringjs";

import {
    LineChart,
    Baseline,
    ChartContainer,
    ChartRow,
    EventChart,
    YAxis,
    Charts
} from "react-timeseries-charts";

import { TimeSeries, Index, TimeEvent, TimeRange, Stream } from "pondjs";

class Gnss extends Component {

    constructor(props) {
        super(props);
        this.state = {
            events: new Ring(2000),
            gnss: new Ring(2000),
            stream: new Stream(),
            firstEvent: 0,
            lastEvent: 0
        };
    }

    componentDidUpdate(prevProps) {
        if (prevProps.state.altitude !== this.props.state.altitude) {
            console.log("Altitude changed!");
            let event = new TimeEvent(Date.now(), Number(this.props.state.altitude));

            const newEvents = this.state.events;
            newEvents.push(event);
            this.setState({ events: newEvents, lastEvent: Date.now() });

            let event2 = new TimeEvent(Date.now(), Number(this.props.state.gnss_fix_quality));

            const newEvents2 = this.state.gnss;
            newEvents2.push(event2);
            this.setState({ gnss: newEvents2});

            if (this.state.firstEvent == 0) {
                this.setState({ firstEvent: Date.now() });
            }
        }
    }

    componentDidMount() {

    }

    render() {
        const series = new TimeSeries({
            name: "raw",
            events: this.state.events.toArray()
        });

        const series2 = new TimeSeries({
            name: "raw",
            events: this.state.gnss.toArray()
        });

        const timeRange = new TimeRange(this.state.firstEvent, this.state.lastEvent);

        return (
            <div>
                <h3>GNSS Dashboard</h3>
                <p>Latitude: {(this.props.state.latitude*1.1231).toFixed(6)}</p>
                <p>Longitude: {(this.props.state.longitude*1.1231).toFixed(6)}</p>
                <p>True North (d): {this.props.state.true_north}</p>
                <p>Ground Speed (kph): {this.props.state.ground_speed}</p>
                <p>Altitude (m): {this.props.state.altitude}</p>
                <hr></hr>
                <p>GNSS Fix Quality: {this.props.state.gnss_fix_quality}</p>
                <p>GNSS Fix Type: {this.props.state.gnss_fix_type}</p>
                <p>GPS Satellites Count: {this.props.state.sat_count_gps}</p>
                <p>GLONASS Satellites Count: {this.props.state.sat_count_glonass}</p>
                <br></br>
                <ChartContainer 
                    title="Altitude over Time"
                    format="relative"
                    timeRange={timeRange}>
                    <ChartRow height="150">
                        <YAxis
                            id="y"
                            min={series.min()}
                            max={series.max()}
                            label="Altitude (m)"
                            format=".1f"
                            type="linear"/>
                        <Charts>
                            <Baseline axis="y" value={series.max()} label="Max"/>
                            <Baseline axis="y" value={series.min()} label="Min"/>
                            <LineChart axis="y" series={series} />
                            <Baseline axis="y" value={series.avg()} label="Avg"/>
                            <EventChart series={series2} label="DGNSS" />
                        </Charts>
                    </ChartRow>
                </ChartContainer>
            </div>
        )
    }

}

Gnss.propTypes = rxa.props
export default connect(rxa.mapStateToProps)(Gnss)  
