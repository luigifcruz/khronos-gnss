import HeatmapLayer from "react-google-maps/lib/components/visualization/HeatmapLayer";
import { TimeSeries, Index, TimeEvent, TimeRange } from "pondjs";
import React, { Component } from 'react'
import { compose, withProps } from 'recompose'
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

import {
  withScriptjs,
  withGoogleMap,
  GoogleMap,
  Marker
} from "react-google-maps";

const MyMapComponent = compose(
  withProps({
    googleMapURL: "https://maps.googleapis.com/maps/api/js?key=AIzaSyA8-WppouVH_aiun-Poq89TAxhRhIj1y4Y&libraries=visualization",
    loadingElement: <div style={{ height: `100%` }} />,
    containerElement: <div style={{ height: `400px` }} />,
    mapElement: <div style={{ height: `100%` }} />,
  }),
  withScriptjs,
  withGoogleMap
)((props) =>
    <GoogleMap
        defaultZoom={16} 
        options={{ scrollwheel: true }} 
        defaultCenter={{ lat: props.lat, lng: props.lng }} >
        <Marker position={{ lat: props.lat, lng: props.lng }} />
        <HeatmapLayer data={props.loc.toArray()}/>
    </GoogleMap>
)

class Gnss extends Component {

    constructor(props) {
        super(props);
        this.state = {
            altitude: new Ring(2000),
            speed: new Ring(2000),
            location: new Ring(100),
            firstEvent: 0,
            lastEvent: 0
        };
    }

    digestData() {
        let { state } = this.props;

        const { altitude } = this.state;
        altitude.push(new TimeEvent(Date.now(), state.altitude));
        this.setState({ altitude });

        const { speed } = this.state;
        speed.push(new TimeEvent(Date.now(), state.ground_speed));
        this.setState({ speed });

        const { location } = this.state;
        const htmapPoint = {
            location: new google.maps.LatLng(
                state.latitude,
                state.longitude
            ), 
            weight: 1
        };
        location.push(htmapPoint);
        this.setState({ location });

        this.setState({ lastEvent: Date.now() });
    }

    componentDidMount() {
        this.setState({ firstEvent: Date.now() });
        this.timer = setInterval(() => { this.digestData() }, 1000)
    }

    componentWillUnmount() {
        clearInterval(this.timer);
    }

    render() {
        const altitude = new TimeSeries({
            name: "raw",
            events: this.state.altitude.toArray()
        });

        const speed = new TimeSeries({
            name: "raw",
            events: this.state.speed.toArray()
        });

        const timeRange = new TimeRange(this.state.firstEvent, this.state.lastEvent);

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
                <br></br>
                <ChartContainer 
                    title="Altitude"
                    format="relative"
                    timeRange={timeRange}>
                    <ChartRow height="150">
                        <YAxis
                            id="y"
                            min={altitude.min()}
                            max={altitude.max()}
                            label="Altitude (m)"
                            format=".1f"
                            type="linear"/>
                        <Charts>
                            <Baseline axis="y" value={altitude.max()} label="Max"/>
                            <Baseline axis="y" value={altitude.min()} label="Min"/>
                            <LineChart axis="y" series={altitude} />
                            <Baseline axis="y" value={altitude.avg()} label="Avg"/>
                        </Charts>
                    </ChartRow>
                </ChartContainer>
                <ChartContainer 
                    title="Ground Speed"
                    format="relative"
                    timeRange={timeRange}>
                    <ChartRow height="150">
                        <YAxis
                            id="y"
                            min={speed.min()}
                            max={speed.max()}
                            label="Ground Speed (kph)"
                            format=".1f"
                            type="linear"/>
                        <Charts>
                            <LineChart axis="y" series={speed} />
                        </Charts>
                    </ChartRow>
                </ChartContainer>
                <MyMapComponent
                    lat={this.props.state.latitude}
                    lng={this.props.state.longitude}
                    loc={this.state.location} />
            </div>
        )
    }

}

Gnss.propTypes = rxa.props
export default connect(rxa.mapStateToProps)(Gnss)  
