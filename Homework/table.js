document.addEventListener("ready", function(){
	var tables = document.getElementsByClassName("sort_table");
	for( var table_index in tables ){
		table = tables[table_index];
		var head = table.getElementsByTagName("thead")[0];
		var body = table.getElementsByTagName("tbody")[0];
		
		var titles = head.getElementsByTagName("tr")[0].getElementsByTagName("th");
		for( var title_index = 0 ; title_index < titles.length ; title_index++ ){
			if( titles[title_index].className )
		}
		
	}
	
});