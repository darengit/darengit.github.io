var height = getHeight();
var width = getWidth();
var centerX = Math.floor(width/2);
var centerY = Math.floor(height/2);
var resetRadius = 100;
var radius = resetRadius;
var radiusDelta = 3;
var period = 20;
var score = 0;

var intervalId = null;

function runAPMClick() {
	// set initial score to 0
	document.getElementById("score").innerHTML = score;

	// draw circle in a loop, increasing speed
	intervalId = setInterval(shrinkCircle,period);

	function shrinkCircle(){
		targetElem = document.getElementById("target");
		targetElem.style.width = radius*2 + "px";
		targetElem.style.height = radius*2 + "px";
		targetElem.style.borderRadius = radius + "px";
		targetElem.style.position = "relative"; 
		targetElem.style.left = centerX - radius + "px";
		targetElem.style.top = centerY - radius + "px";
		targetElem.style.background = "red"; 


		// if the target becomes 0 then game is over
		if (--radius == 0) {
			gameOver();
		}
	}
}

document.body.addEventListener('click', gameOver); 
document.getElementById("target").addEventListener('click', targetClickCB);

function gameOver() {
	clearInterval(intervalId)
	document.getElementById("target").innerHTML = "GAME OVER";
}

function targetClickCB(clickEvent) {
	clickEvent.stopImmediatePropagation();
	resetRadius = resetRadius - radiusDelta;
	if (resetRadius < 0) {
		gameOver();
	}
	radius = resetRadius;
	centerX = getRandomInt(radius, width - radius)
	centerY = getRandomInt(radius, height - radius)
	document.getElementById("score").innerHTML = ++score
	
}

function getWidth() {
  return Math.max(
    document.body.scrollWidth,
    document.documentElement.scrollWidth,
    document.body.offsetWidth,
    document.documentElement.offsetWidth,
    document.documentElement.clientWidth
  );
}

function getHeight() {
  return Math.max(
    document.body.scrollHeight,
    document.documentElement.scrollHeight,
    document.body.offsetHeight,
    document.documentElement.offsetHeight,
    document.documentElement.clientHeight
  );
}

function getRandomInt(min, max) {
  min = Math.ceil(min);
  max = Math.floor(max);
  return Math.floor(Math.random() * (max - min) + min); // The maximum is exclusive and the minimum is inclusive
}
