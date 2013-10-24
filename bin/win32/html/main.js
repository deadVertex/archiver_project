function CreateTreeNode( parent, jsonEntry ) {
	var node = parent.addChild( {
		title: jsonEntry.name,
		tooltip: jsonEntry.name,
		key: jsonEntry.name,
		isFolder: true
	} );
	if ( jsonEntry.children ) {
		$.each( jsonEntry.children, function( key, value ) {
			CreateTreeNode( node, value );
		} );
	}
}

function LoadTreeData( json ) {
	var rootNode = $("#tree").dynatree("getRoot");
	var data = $.parseJSON( json );
	$.each( data, function( key, value ) {
		CreateTreeNode( rootNode, value );
	});
}

$(function() {
	$( "#file-menu" ).menu();
	$( "#file-menu" ).hide();
	
	$( "#file-menu-button" ).click( function(e) {
		$( "#file-menu" ).show();
		e.stopPropagation();
	} );
	
	$("body").click( function() {
		$( "#file-menu" ).hide();
	} );
	
	$("#tree").dynatree({
		imagePath: "css/dynatree/",
		dnd: {
			revert: true,
			onDragStart: function(node) {
				if(node.data.isFolder) {
					return false;
				}
				return true;
			},
			onDragStop: function(node) {
				logMsg( "tree.onDragStop( %o )", node );
			}
		}
	});
	
	//LoadTreeData( GetTreeData( ) );
	// Some hardcoded test values for use when page displayed in browser.
	LoadTreeData( '[ { "name" : "folder1", "children" : [ { "name" : "folder2" } ] }, { "name" : "folder3" } ]' );
});