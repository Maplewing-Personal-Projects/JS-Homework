(function main(){
    var inputs = readline().split(" ").map(function(x) { return parseInt(x); });
    var n = inputs[0], m = inputs[1], a = inputs[2];

    print(Math.ceil(n/a)*Math.ceil(m/a));
})();