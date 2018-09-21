import React, { Component, PropTypes } from 'react'
import * as rxa from '../redux/actions'
import { connect } from 'react-redux'
import '../styles/Gnss.scss'

import {
    Charts,
    ChartContainer,
    BarChart,
    Resizable,
    ChartRow,
    YAxis,
    styler,
    LineChart
} from "react-timeseries-charts";

import { TimeSeries, Index } from "pondjs";

const data = [["2018-09-21T05:39:17.200Z","730.599976"],["2018-09-21T05:39:17.220Z","731.000000"],["2018-09-21T05:39:18.131Z","732.200012"],["2018-09-21T05:39:19.128Z","732.500000"],["2018-09-21T05:39:20.130Z","732.599976"],["2018-09-21T05:39:21.127Z","733.200012"],["2018-09-21T05:39:22.130Z","733.799988"],["2018-09-21T05:39:23.126Z","734.000000"],["2018-09-21T05:39:24.128Z","734.599976"],["2018-09-21T05:39:25.138Z","734.900024"],["2018-09-21T05:39:27.124Z","735.099976"],["2018-09-21T05:39:28.126Z","735.299988"],["2018-09-21T05:39:29.122Z","735.500000"],["2018-09-21T05:39:30.149Z","735.599976"],["2018-09-21T05:39:31.123Z","735.500000"],["2018-09-21T05:39:32.122Z","735.599976"],["2018-09-21T05:39:33.128Z","736.000000"],["2018-09-21T05:39:34.141Z","736.299988"],["2018-09-21T05:39:35.127Z","736.799988"],["2018-09-21T05:39:36.158Z","737.000000"],["2018-09-21T05:39:37.123Z","737.200012"],["2018-09-21T05:39:38.149Z","737.299988"],["2018-09-21T05:39:39.122Z","737.200012"],["2018-09-21T05:39:40.137Z","737.000000"],["2018-09-21T05:39:41.123Z","736.700012"],["2018-09-21T05:39:42.137Z","736.500000"],["2018-09-21T05:39:43.135Z","737.000000"],["2018-09-21T05:39:44.128Z","737.099976"],["2018-09-21T05:39:45.128Z","737.200012"],["2018-09-21T05:39:46.146Z","737.500000"],["2018-09-21T05:39:48.145Z","738.000000"],["2018-09-21T05:39:49.148Z","736.700012"],["2018-09-21T05:39:50.128Z","735.200012"],["2018-09-21T05:39:51.127Z","733.700012"],["2018-09-21T05:39:52.149Z","732.000000"],["2018-09-21T05:39:53.132Z","729.799988"],["2018-09-21T05:39:54.132Z","728.200012"],["2018-09-21T05:39:55.139Z","727.099976"],["2018-09-21T05:39:56.147Z","726.099976"],["2018-09-21T05:39:57.127Z","724.900024"],["2018-09-21T05:39:58.134Z","723.799988"],["2018-09-21T05:39:59.131Z","723.000000"],["2018-09-21T05:40:00.125Z","722.000000"],["2018-09-21T05:40:01.132Z","721.400024"],["2018-09-21T05:40:02.130Z","720.700012"],["2018-09-21T05:40:03.141Z","720.099976"],["2018-09-21T05:40:04.136Z","719.400024"],["2018-09-21T05:40:05.128Z","718.599976"],["2018-09-21T05:40:06.125Z","718.200012"],["2018-09-21T05:40:07.127Z","717.299988"],["2018-09-21T05:40:08.131Z","716.599976"],["2018-09-21T05:40:09.143Z","715.900024"],["2018-09-21T05:40:10.125Z","715.299988"],["2018-09-21T05:40:11.131Z","714.700012"],["2018-09-21T05:40:12.126Z","714.200012"],["2018-09-21T05:40:13.123Z","713.700012"],["2018-09-21T05:40:14.123Z","713.000000"],["2018-09-21T05:40:15.122Z","712.700012"],["2018-09-21T05:40:16.120Z","712.200012"],["2018-09-21T05:40:17.124Z","711.599976"],["2018-09-21T05:40:18.126Z","710.799988"],["2018-09-21T05:40:19.147Z","710.299988"],["2018-09-21T05:40:20.126Z","709.799988"],["2018-09-21T05:40:21.122Z","709.200012"],["2018-09-21T05:40:22.125Z","708.400024"],["2018-09-21T05:40:23.123Z","707.700012"],["2018-09-21T05:40:24.143Z","706.900024"],["2018-09-21T05:40:25.127Z","706.200012"],["2018-09-21T05:40:26.154Z","705.299988"],["2018-09-21T05:40:27.126Z","704.500000"],["2018-09-21T05:40:28.127Z","703.900024"],["2018-09-21T05:40:29.122Z","703.299988"],["2018-09-21T05:40:30.129Z","702.700012"]]
const series = new TimeSeries({
    name: "hilo_rainfall",
    columns: ["index", "precip"],
    points: data.map(([d, value]) => [Index.getIndexString("1h", new Date(d)), value])
});


class Gnss extends Component {

    constructor(props) {
        super(props);
        this.state = {
            altitudeData: [],
            startTime: 0,
            endTime: 0
        };
    }

    componentDidUpdate(prevProps) {
        if (prevProps.state.altitude !== this.props.state.altitude) {
            console.log("Altitude changed!");
            let { altitudeData } = this.state;    
            altitudeData.push([ new Date(), this.props.state.altitude ]);
            this.setState({ altitudeData, endTime: Math.floor(Date.now() / 1000) });
            console.log(JSON.stringify(this.state.altitudeData));

            if (this.state.startTime == 0) {
                this.setState({ startTime: new Date() });
            }
        }
    }

    render() {
        const style = styler([{ key: "precip", color: "#A5C8E1", selected: "#2CB1CF" }]);
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
                <Resizable>
                            <ChartContainer timeRange={series.range()}>
                                <ChartRow height="150">
                                    <YAxis
                                        id="rain"
                                        label="Rainfall (inches/hr)"
                                        min={0}
                                        max={1.5}
                                        format=".2f"
                                        width="70"
                                        type="linear"
                                    />
                                    <Charts>
                                        <BarChart
                                            axis="rain"
                                            style={style}
                                            spacing={1}
                                            columns={["precip"]}
                                            series={series}
                                            minBarHeight={1}
                                        />
                                    </Charts>
                                </ChartRow>
                            </ChartContainer>
</Resizable>
            </div>
        )
    }

}

Gnss.propTypes = rxa.props
export default connect(rxa.mapStateToProps)(Gnss)  
