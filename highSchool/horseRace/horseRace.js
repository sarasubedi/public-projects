var h = 700; // height of bg
var leftPos = 466; // left position of horse
var topPos = 554; // top position of horse
var speed = 50;
var moving;
var collisions;
var counter = 0;
var createObs; // create obstacles
var randInt; // number of miliseconds between creating objects
var obstacles = [{top: 0, left: null}, {top: 0, left: null}, {top: 0, left: null}, {top: 0, left: null}];
var obsExist = [false, false, false, false];
var gameLength;

// change what is visible to the player //

function changeScreen (screen) {
	
  if (screen == 1) {
	// start to game options //
    document.getElementById("startScreen").style.display = "none";
	document.getElementById("gameOption").style.display = "block";
  } else if (screen == 2) {
	// start to instructions //
	document.getElementById("startScreen").style.display = "none";
    document.getElementById("instructions").style.display = "block";
  } else if (screen == 3) {
	// code for credits //
  } else if (screen == 4) {
	// instructions to start //
	document.getElementById("instructions").style.display = "none";
    document.getElementById("startScreen").style.display = "block";
  } else if (screen == 5) {
	// game options to start //
	document.getElementById("gameOption").style.display = "none";
	document.getElementById("startScreen").style.display = "block";
  } else if (screen == 6) {
	document.getElementById("gameOption").style.display = "none";
    document.getElementById("play").style.display = "block";
	document.getElementById("gameScreen").style.display = "block";
	createObject();
	gameLength = 1;
    moving = setInterval(moveBackground, speed);
	randInt = getRand();
	createObs = setInterval(createObject, randInt);
	collisions = setInterval(checkCollision, 10);
    move();
  } else if (screen == 7) {
	document.getElementById("gameOption").style.display = "none";
    document.getElementById("play").style.display = "block";
	document.getElementById("gameScreen").style.display = "block";
	createObject();
	gameLength = 2;
    moving = setInterval(moveBackground, speed);
	randInt = getRand();
	createObs = setInterval(createObject, randInt);
	collisions = setInterval(checkCollision, 10);
    move();
  } else if (screen == 8) {
	document.getElementById("gameOption").style.display = "none";
    document.getElementById("play").style.display = "block";
	document.getElementById("gameScreen").style.display = "block";
	createObject();
	gameLength = 3;
    moving = setInterval(moveBackground, speed);
	randInt = getRand();
	createObs = setInterval(createObject, randInt);
	collisions = setInterval(checkCollision, 10);
    move();
  } else if (screen == 9) {
	document.getElementById("gameOption").style.display = "none";
    document.getElementById("play").style.display = "block";
	document.getElementById("gameScreen").style.display = "block";
	createObject();
	gameLength = 4;
    moving = setInterval(moveBackground, speed);
	randInt = getRand();
	createObs = setInterval(createObject, randInt);
	collisions = setInterval(checkCollision, 10);
    move();
  } else if (screen == 10) {
	document.getElementById("gameOption").style.display = "none";
    document.getElementById("play").style.display = "block";
	document.getElementById("gameScreen").style.display = "block";
	createObject();
	gameLength = 5;
    moving = setInterval(moveBackground, speed);
	randInt = getRand();
	createObs = setInterval(createObject, randInt);
	collisions = setInterval(checkCollision, 10);
	move();
    
  } // else if
  
} // changeScreen

// moves the horse forwards, backwards, left and right with 'W', 'S', 'A' and 'D' //

function move() {
	
  window.addEventListener('keypress', function (e) {
    //var c = String.fromCharCode(e.keyCode);
	var horse = document.getElementById("horse");
    if (e.key == 'a' && leftPos >= 200) {
	  leftPos -= 5;
	} else if (e.key == 'd' && leftPos <= 768) {
	  leftPos += 5;
	}
	if (e.key == 'w' && topPos >= 0) {
	  topPos -= 5;
	} else if (e.key == 's' && topPos <= 554) {
	  topPos += 5;
	}
	horse.style.left = leftPos + "px";
	horse.style.top = topPos + "px";
  }, false);
  
} // move

// moves the background, increasing speed as time goes on //

function moveBackground () {
  var x = document.getElementById("play");
  h += 5;
  x.style.height = h + "px";
    
	console.log(gameLength);
  
  // increases the speed of the background //
  if (counter % 100 == 0 && speed != 20) {
    speed -= 5;
	//console.log('faster');
	clearInterval(moving);
	moving = setInterval(moveBackground, speed);
  } // if
  
  if (gameLength == 1 && h >= 4000) {
	  alert('you made it!');
  } else if (gameLength == 2 && h >= 5400) {
	  alert('you made it!');
  } else if (gameLength == 3 && h >= 9000) {
	  alert('you made it!');
  } else if (gameLength == 4 && h >= 12000) {
	  alert('you made it!');
  } else if (gameLength == 5 && h >= 20000) {
	  alert('you made it!');
  }
  
  counter++;
  moveObstacles();
} // moveBackground

// moves obstacles at the same rate as the background //
// deleting them if they go off the screen //

function moveObstacles () {
  //obstacles = [{top: 0, left: null}, {top: 0, left: null}, {top: 0, left: null}, {top: 0, left: null}]
  
  for(var j = 0; j < obstacles.length; j ++){
	  var x = document.getElementById('obstacle' + j);
	  
	  if(obsExist[j]){
		 
		  obstacles[j].top += 5;
		  x.style.top = obstacles[j].top + 'px';  // add to position
		
	  } // if
	  
	  // remove log if offscreen
	  if(obstacles[j].top > 700){
		  // remove image, reset top and left and obsExist
		  obsExist[j] = false;
		  obstacles[j].top = 0;
		  obstacles[j].left = null;
		  
		  // x is the log
		  var father = document.getElementById('gameScreen');
		  
		  father.removeChild(x);
		  
		  
		  
		  
	  } // if
	  
  } // for
  
} // moveObstacles

// creates the obstacles, only four on screen at a time //

function createObject() {
	
  if(!obsExist[0] || !obsExist[1] || !obsExist[2] || !obsExist[3]){
	  // objects come down from three columns at intervals of 0-4 seconds //
	  // http://pixelartmaker.com/art/8ef89d2669b8851 //
	  var oImg = document.createElement("img");
	  var randLeft = (Math.floor(Math.random() * 5 + 2) * 100);
	  
	  oImg.setAttribute('src', 'images/0.png');
	  oImg.setAttribute('alt', 'obstacle');
	  oImg.style.position = "absolute";
	  oImg.style.left = '500px';
	  oImg.style.width = '125px';
	  oImg.style.zIndex = 99;
	  oImg.style.display = 'block';
	  oImg.style.left = randLeft + "px";
	  
	  if(!obsExist[0]){
		  oImg.id = 'obstacle0';
		  obstacles[0].left = randLeft;
		  obsExist[0] = true;
	  } else if (!obsExist[1]) {
		  oImg.id = 'obstacle1';
		  obstacles[1].left = randLeft;
		  obsExist[1] = true;
	  } else if (!obsExist[2]) {
		  oImg.id = 'obstacle2';
		  obstacles[2].left = randLeft;
		  obsExist[2] = true;
	  } else if (!obsExist[3]) {
		  oImg.id = 'obstacle3';
		  obstacles[3].left = randLeft;
		  obsExist[3] = true;
	  } // else if
	  
	  document.getElementById('gameScreen').appendChild(oImg);
  } // if
} // createObject

// checks if horse has collided with any obstacles //

function checkCollision(){
  // eventually use an array to encompass all obstacles
  
  for(var j = 0; j < obstacles.length; j++){
	
	//console.log(obstacles[j].left);
	if (leftPos + 52 > obstacles[j].left && leftPos < obstacles[j].left + 125){
		
		if (topPos > obstacles[j].top && topPos < obstacles[j].top + 45) {
			
			if(speed == 50){
				alert("You lose");
				
			} // if
			
			speed = 50;
			counter = 0;
			
			console.log("collide");
		} // if
    } // if
  } // for
	
} // checkCollision

// get a random number between 3000 and 500 //

function getRand () {
  return Math.floor(Math.random() * (3000 - 500) + 500);
}

/*
call a function and pass it a random integer
function pauses for that long
calls createObject

*/