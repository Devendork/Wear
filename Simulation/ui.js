//assume 60 frames per second.

// var data_front = data_front_12921;
// var data_back = data_back_12921;

 var data_front = data_front_13021;
 var data_back = data_back_13021;


function initUI(){

	var delayInMilliseconds = 5; //1 second (real time)
	var count = 0;
	var states_front = [];
	var states_back = [];


	for(var i = 0; i < 11; i++){
		states_back.push(false);
		states_front.push(false);
	}

	var elem = document.getElementById("frame_count");
	elem.innerHTML = data_front.length+" : "+data_back.length;

	elem = document.getElementById("date");
	elem.innerHTML = "Capture Date: 1 / 30 / 21";

	elem = document.getElementById("speed");
	elem.innerHTML = "1 second realtime = "+delayInMilliseconds+" ms screentime";





	let next = function(){

		const elem = document.getElementById("frame_num");
		elem.innerHTML = count;



		if(count < data_front.length){

			for(var j = 0; j < 10; j++){
				const elem = document.getElementById("bobble"+j+"f");
				if(data_front[count][j] > 0){
					 if(!states_front[j]) elem.style.animation = "reveal .5s normal forwards";
					 states_front[j] = true;
				}else{
					if(states_front[j]) elem.style.animation = "hide .5s normal forwards";
				    states_front[j] = false;
				}
			}
		}


		if(count < data_back.length){

			for(var j = 0; j < 10; j++){
				const elem = document.getElementById("bobble"+j);
				if(data_back[count][j] > 0){
					 if(!states_back[j]) elem.style.animation = "reveal .5s normal forwards";
					 states_back[j] = true;
				}else{
					if(states_back[j]) elem.style.animation = "hide .5s normal forwards";
				    states_back[j] = false;
				}
			}
		}

		count++;
		if(count > data_back.length && count > data_front.length) 
		window.clearInterval();
	}

	window.setInterval(next, delayInMilliseconds);


}
