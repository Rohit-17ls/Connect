const terminalConsole = document.querySelector('#console');
const title = document.title;
let messages = terminalConsole.children.length;

window.addEventListener('focusin', (e) => {
	messages = terminalConsole.children.length;
	document.title = title;
});

window.addEventListener('focusout', (e) => {
	messages = terminalConsole.children.length;
	document.title = title;
});

const lookForMessages = () => {
        const currentMessageCount = terminalConsole.children.length;
        if(currentMessageCount > messages){
                document.title = `(${currentMessageCount - messages}) ${title}`;
        }
}

window.addEventListener('focus', (e) => {messages = terminalConsole.children.length;});
setInterval(lookForMessages, 1000);

