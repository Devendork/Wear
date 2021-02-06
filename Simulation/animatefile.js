

var scene;
var precision = 1000;
var num_segments = 5;
var knots = [];
var bobbles = [];
var show_animations = [];

let start;

$( document ).ready(function() {
    console.log( "ready!" );



    function hide(timestamp) {
	  if (start === undefined)
	    start = timestamp;
	    const elapsed = (timestamp - start)/2000;

	    bobbles[0].stroke({opacity: 1-elapsed});


	  // `Math.min()` is used here to make sure that the element stops at exactly 200px.
	  //element.style.transform = 'translateX(' + Math.min(0.1 * elapsed, 200) + 'px)';

	  if (elapsed < 2000) { // Stop the animation after 2 seconds
	    window.requestAnimationFrame(hide);
	  }
	}


    initUI();

});


let readTextFile = function(file)
{
    var rawFile = new XMLHttpRequest();
    rawFile.open("GET", file, false);
    rawFile.onreadystatechange = function ()
    {
        if(rawFile.readyState === 4)
        {
            if(rawFile.status === 200 || rawFile.status == 0)
            {
                var allText = rawFile.responseText;
                alert(allText);
            }
        }
    }
    rawFile.send(null);
}








