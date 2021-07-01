
export default function(command) {
    fetch("/command", {
        method: "POST",
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ command: command })
    })
    .then(resp => {
        return resp.json()
    }) 
    .then(data => {
        console.log(data)
    })
    .catch((error) => {
        console.log(error, "catch the hoop")
    })
};