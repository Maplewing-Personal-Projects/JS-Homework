var MarkusConverter = (function(){
	function MarkusConverter(input){
		this.input = input;
	}

	MarkusConverter.tags = [
		{name: "fullName", attribute: ["cbdbid"], moreThanOneId:0},
		{name: "partialName", attribute: ["cbdbid"], moreThanOneId:0},
		{name: "officialTitle", attribute: ["officialtitle_id"], moreThanOneId:0},
		{name: "placeName", attribute: ["placename_id"], moreThanOneId:0},
		{name: "timePeriod", attribute: ["timeperiod_id"], moreThanOneId:0},
	];

	MarkusConverter.encode = function(str){
		return str.replace(/\%/g, '%25').replace(/\&/g, '%26').replace(/\</g, '%3C').replace(/\>/g, '%3E');
	}

	MarkusConverter.decode = function(str){
		return str.replace(/\%26/g, '&').replace(/\%3C/g, '<').replace(/\%3E/g, '>').replace(/\%25/g, '%');
	}

	MarkusConverter.prototype.plainTextConvertToSTAML = function(){
		var paragraph = this.input.split('\n');
		paragraph = paragraph.map(function(s){ return s.trim(); })
							 .filter(function(s){ if(s !== "") return true; });
		
		var doc = '<STAML>';
		
		for( var i = 0 ; i < paragraph.length ; i++ ){
			doc += "<passage>" + paragraph[i] + "</passage>";
		}

		doc += "</STAML>";

		return this.output = doc;
	}

	MarkusConverter.prototype.plainTextConvertToMarkus = function(){
		var paragraph = this.input.split('\n');
		paragraph = paragraph.map(function(s){ return s.trim(); })
							 .filter(function(s){ if(s !== "") return true; });
		
		var markusDocument = '<div class="doc"><pre>';
		
		for( var i = 0 ; i < paragraph.length ; i++ ){
			markusDocument += '<span class="passage" type="passage" id="passage' + i + '"><span class="commentContainer" value="[]"><span class="glyphicon glyphicon-comment" type="commentIcon" style="display:none" aria-hidden="true" data-markus-passageid="passage' + i + '"></span></span><span>' + paragraph[i] + "</span></span>\n\n";
		}

		markusDocument += "</pre></div>";

		return this.output = markusDocument;
	}

	MarkusConverter.prototype.tagNodeToString = function(node){
		var returnString = "";

		if( node.nodeType === 3 ){
			return MarkusConverter.encode( node.nodeValue );
		}

		for( var i = 0 ; i < MarkusConverter.tags.length ; i++ ){
			if( node.classList.contains(MarkusConverter.tags[i].name) ){
				returnString += '<' + MarkusConverter.tags[i].name;
				for( var j = 0 ; j < MarkusConverter.tags[i].attribute.length ; j++ ){
					if( node.getAttribute(MarkusConverter.tags[i].attribute[j]) !== null ){
						returnString += ' ' + MarkusConverter.tags[i].attribute[j] + '="' 
											+ MarkusConverter.encode( node.getAttribute(MarkusConverter.tags[i].attribute[j]) ) + '"';
					} 
				}
				returnString += '>';

				var childNodes = node.childNodes; 
				for( var j = 0 ; j < childNodes.length ; j++ ){
					if( childNodes[j].nodeType === 1 ||
						childNodes[j].nodeType === 3 ){
						returnString += this.tagNodeToString(childNodes[j]);
					}
				}

				returnString += '</' + MarkusConverter.tags[i].name + '>';
				return returnString;
			}
		}

		var childNodes = node.childNodes; 
		for( var i = 0 ; i < childNodes.length ; i++ ){
			if( childNodes[i].nodeType === 1 ||
				childNodes[i].nodeType === 3 ){
				returnString += this.tagNodeToString(childNodes[i]);
			}
		}

		return returnString;
	}

	MarkusConverter.prototype.markusConvertToSTAML = function(){
		var parser = new DOMParser();
		var markusDoc = parser.parseFromString(this.input, "text/html");
		var passages = markusDoc.getElementsByClassName('passage');
		var paragraph = [];
		for( var i = 0 ; i < passages.length ; i++ ){
			paragraph[i] = '';
			var childNodes = passages[i].childNodes;
			for( var j = 0 ; j < childNodes.length ; j++ ){
				paragraph[i] += this.tagNodeToString(childNodes[j]);
			}
		}

		return this.output = "<STAML><passage>" + paragraph.join('</passage><passage>') + "</passage></STAML>";
	}

	MarkusConverter.prototype.tagNodeToMarkusString = function(node){
		var returnString = "";

		if( node.nodeType === 3 ){
			return MarkusConverter.decode( node.nodeValue );
		}

		for( var i = 0 ; i < MarkusConverter.tags.length ; i++ ){
			if( node.nodeName.toLowerCase() === MarkusConverter.tags[i].name.toLowerCase() ){
				returnString += '<span class="' + MarkusConverter.tags[i].name + ' markup unsolved';
				var moreThanOneAttribute = node.getAttribute(MarkusConverter.tags[i].attribute[MarkusConverter.tags[i].moreThanOneId]);
				if( moreThanOneAttribute != null && moreThanOneAttribute.split("|").length > 1 ){
					returnString += ' moreThanOneId';
				}
				returnString += '" type="' + MarkusConverter.tags[i].name + '"';

				for( var j = 0 ; j < MarkusConverter.tags[i].attribute.length ; j++ ){
					if( node.getAttribute(MarkusConverter.tags[i].attribute[j]) !== null ){
						returnString += ' ' + MarkusConverter.tags[i].attribute[j] + '="' 
											+ MarkusConverter.decode( node.getAttribute(MarkusConverter.tags[i].attribute[j]) ) + '"';
					} 
				}
				returnString += '>';

				var childNodes = node.childNodes; 
				for( var j = 0 ; j < childNodes.length ; j++ ){
					if( childNodes[j].nodeType === 1 ||
						childNodes[j].nodeType === 3 ){
						returnString += this.tagNodeToMarkusString(childNodes[j]);
					}
				}

				returnString += '</span>';
				return returnString;
			}
		}

		var childNodes = node.childNodes; 
		for( var i = 0 ; i < childNodes.length ; i++ ){
			if( childNodes[i].nodeType === 1 ||
				childNodes[i].nodeType === 3 ){
				returnString += this.tagNodeToString(childNodes[i]);
			}
		}

		return returnString;
	}

	MarkusConverter.prototype.STAMLConvertToMarkus = function(){
		var parser = new DOMParser();
		var xmlDoc = parser.parseFromString(this.input, "text/xml");
		
		var markusDocument = '<div class="doc"><pre>';
		var passages = xmlDoc.getElementsByTagName("passage");

		for( var i = 0 ; i < passages.length ; i++ ){
			markusDocument += '<span class="passage" type="passage" id="passage' + i + '"><span class="commentContainer" value="[]"><span class="glyphicon glyphicon-comment" type="commentIcon" style="display:none" aria-hidden="true" data-markus-passageid="passage' + i + '"></span></span><span>';

			var childNodes = passages[i].childNodes;
			for( var j = 0 ; j < childNodes.length ; j++ ){
				markusDocument += this.tagNodeToMarkusString(childNodes[j]);
			}

			markusDocument += '</span></span>\n\n';
		}

		markusDocument += '</pre></div>';

		return this.output = markusDocument;
	}

	return MarkusConverter;
})();