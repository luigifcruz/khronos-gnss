import React, { Component } from 'react'
import { CopyToClipboard } from 'react-copy-to-clipboard'

class Variables extends Component {

    constructor(props) {
        super(props);
        this.state = {
            gnssFixType: "#E54B4B",
            gnssFixTypeColor: "#E54B4B",
            gnssFixQuality: "#E54B4B",
            gnssFixQualityColor: "#E54B4B",
        }
    }

    updateValues() {
        const { gnss_fix_type, gnss_fix_quality } = this.props.state;

        if (gnss_fix_type === 1 || gnss_fix_type === 0) {
            this.setState({ gnssFixType: "NO FIX", gnssFixTypeColor: "#E54B4B" });
        } else if (gnss_fix_type === 2) {
            this.setState({ gnssFixType: "2D FIX", gnssFixTypeColor: "#FDE74C" });
        } else if (gnss_fix_type === 3) {
            this.setState({ gnssFixType: "3D FIX", gnssFixTypeColor: "#4EA699" });
        }

        if (gnss_fix_quality === 0) {
            this.setState({ gnssFixQuality: "NO FIX", gnssFixQualityColor: "#E54B4B" });
        } else if (gnss_fix_quality === 1) {
            this.setState({ gnssFixQuality: "GNSS", gnssFixQualityColor: "#6A7FDB" });
        } else if (gnss_fix_quality === 2) {
            this.setState({ gnssFixQuality: "DGNSS", gnssFixQualityColor: "#4EA699" });
        } else if (gnss_fix_quality === 6) {
            this.setState({ gnssFixQuality: "DRGNSS", gnssFixQualityColor: "#FFD400" });
        }
    }

    componentWillReceiveProps(nextProps) {
        const { gnss_fix_type, gnss_fix_quality } = this.props;
        if (nextProps.state !== this.props.state) {
            this.updateValues();
        }
    }

    componentDidMount() {
        this.updateValues();
    }

    render() {
        let { gnssFixType, gnssFixTypeColor, gnssFixQuality, gnssFixQualityColor } = this.state;

        return (
            <div className="Variables">
                <div>
                    <div className="FloatNarrow NoHover">
                        <p style={{ color: gnssFixTypeColor }}>{gnssFixType}</p>
                        <h5>FIX TYPE</h5>
                    </div>
                    <div className="FloatTiny NoHover">
                        <p>{this.props.state.sat_count_glonass}</p>
                        <h5>GLONASS SATS</h5>
                    </div>
                    <CopyToClipboard text={this.props.state.altitude}>
                        <div className="FloatNarrow">
                            <p>{this.props.state.altitude.toFixed(2)}</p>
                            <h5>ALTITUDE (m)</h5>
                        </div>
                    </CopyToClipboard>
                    <CopyToClipboard text={this.props.state.latitude}>
                        <div className="FloatWide">
                            <p>{(this.props.state.latitude*1.132).toFixed(6)}</p>
                            <h5>LATITUDE</h5>
                        </div>
                    </CopyToClipboard>
                </div>
                <div>
                    <div className="FloatNarrow NoHover">
                        <p style={{ color: gnssFixQualityColor }}>{gnssFixQuality}</p>
                        <h5>FIX QUALITY</h5>
                    </div>
                    <div className="FloatTiny NoHover">
                        <p>{this.props.state.sat_count_gps}</p>
                        <h5>GPS SATS</h5>
                    </div>
                    <CopyToClipboard text={this.props.state.ground_speed}>
                        <div className="FloatNarrow">
                            <p>{this.props.state.ground_speed.toFixed(2)}</p>
                            <h5>SPEED (km/h)</h5>
                        </div>
                    </CopyToClipboard>
                    <CopyToClipboard text={this.props.state.longitude}>
                        <div className="FloatWide">
                            <p>{(this.props.state.longitude*1.231).toFixed(6)}</p>
                            <h5>LONGITUDE</h5>
                        </div>
                    </CopyToClipboard>
                </div>
            </div>
        )
    }

}

export default Variables
