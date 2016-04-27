var Calculate = (function(){
	function Calculate(a,b){
		this.a = a;
		this.b = b;
	}

	Calculate.prototype.add = function(){
		return this.a + this.b;
	}

	return Calculate;
})();