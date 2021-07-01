import React from "react";

import "../css/terminal.css";
import Paper from '@material-ui/core/Paper';
import TextField from '@material-ui/core/TextField';
import sendCommand from "../send.js";

class LogTerminal extends React.Component {
  constructor (props) {
    super(props);
    this.terminalEndRef = React.createRef();
  }

  scrollToBottom = () => {
    this.terminalEndRef.current.scrollIntoView({
      behavior: 'smooth', block: 'nearest', inline: 'start'
    });
    // to avoid moving a whole page
    this.terminalEndRef.current.parentNode.scrollTop = this.terminalEndRef.current.offsetTop;
  }

  componentDidMount() {
    this.scrollToBottom();
  }

  componentDidUpdate() {
    this.scrollToBottom();
  }

  keyPress(e){
      if(e.keyCode == 13){
         console.log('value', e.target.value);
         if(e.target.value!=="") {
          sendCommand(e.target.value)
        }
      }
   }

  render() {
    return (
      <>
      <Paper>
        <TextField onKeyDown={this.keyPress} label="Command" fullWidth/>
        <div className="log-terminal">
          {this.props.logs.map((log, i) => {
            var textColor = '';

            switch(log[0]){
              case 0:
                textColor = 'red'
                break;
              case 1:
                textColor = 'powderblue'
                break;
              case 2:
                textColor = 'white'
                break;
              default:
                textColor = 'grey'
            }

            return <p style={{color: textColor}} key={i}>{log[1]}</p>
          })}
          <div ref={this.terminalEndRef} />
        </div>
      </Paper>
      </>
    );
  }
}

export default LogTerminal;
