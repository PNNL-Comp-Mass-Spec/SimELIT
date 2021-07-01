import React from "react";
// import io from 'socket.io-client'
// import LogTerminal from "./LogTerminal.js";
import Button from '@material-ui/core/Button';
import { theme, buttonTheme } from "../theme.js";
import { MuiThemeProvider } from "@material-ui/core/styles";

// const namespace = '/openfoam';
// const socket = io('http://'+document.domain+':5000'+namespace);

class Simulation extends React.Component {
  constructor (props) {
    super(props);
    this.state = {
      taskID : "",
      taskStatus : ""
    }
  }

  run = () => {
    const that = this;
    let body = {
      solver: this.props.solver,
      case: this.props.case
    }

    fetch("/start", {
      method: "POST",
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify(body)
    })
    .then(resp => {
      return resp.json()
    }) 
    .then(data => {
      that.setState({
        taskID : data.task_id,
        taskStatus : data.queue_state
      });
      console.log(that.state)
    })
    .catch((error) => {
      console.log(error, "catch the hoop")
    })
  }

  stop = () => {
    const that = this;
    let body = {
      task_id: that.state.taskID
    }

    fetch("/stop", {
      method: "POST",
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify(body)
    })
    .then(resp => {
      return resp.json()
    }) 
    .then(data => {
      that.setState({
        taskStatus : data.queue_state
      });
      console.log(that.state)
    })
    .catch((error) => {
      console.log(error, "catch the hoop")
    })
  }

  componentDidMount() {
    window.addEventListener('beforeunload', this.NotifyOnLeaving);
    window.addEventListener('unload', this.StopOnLeave);
  }

  componentWillUnmount() {
    window.removeEventListener('beforeunload', this.NotifyOnLeaving);
    window.addEventListener('unload', this.StopOnLeave);
  }

  NotifyOnLeaving= (e) => {
    var message = 'Leaving this page will stop a running simulation.';
    e.returnValue = message;
    return message;
  }

  StopOnLeave = () => {
    const that = this;

    if(that.state.taskStatus === "PENDING" || that.state.taskStatus === "PROGRESS")
    {
      // let body = {
      //   task_id: that.state.taskID
      // }
      // navigator.sendBeacon("/stop", JSON.stringify(body));
      this.stop();
    }
  }

  render() {
    return (
      <>
      <MuiThemeProvider theme={buttonTheme}>
        <Button color="primary" onClick={this.run} disabled={this.state.taskStatus === "PENDING"}>Start</Button>
        <Button color="secondary" onClick={this.stop} disabled={this.state.taskStatus === "ABORTED"}>Stop</Button>
        {/* <Button color="tertiary" onClick={this.stop} disabled={this.state.taskStatus === "ABORTED"}>Pause</Button> */}
      </MuiThemeProvider>
      </>
    );
  }
}

export default Simulation;
