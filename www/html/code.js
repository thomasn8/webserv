let title = document.querySelector('.title');
const colors = ['red', 'green', 'blue', 'orange', 'yellow'];

console.log("Je suis le js");
console.log(title);

title.addEventListener("click", () => {
    title.style.color = colors[Math.floor(Math.random() * colors.length)];
});