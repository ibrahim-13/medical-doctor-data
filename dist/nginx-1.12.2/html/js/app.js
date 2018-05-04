    //Create a module and create a controller for angular
        var myangularapp = angular.module("myangularapp", []);
myangularapp.controller("myangularController", function($scope,$http) {

	/**
		Clear input fileds
	**/
	var clearInputFields = function() {
		$('#name').val('');
		$('#id').val('');
		$('#cgpa').val('');
		$('#index').val('');
	}
	
	
	$scope.bchain = function (){ 
    var lindex;
	var i;
	var mindex="";
	$http.get('http://127.0.0.1:80/api/blockchain/index', {}).then(function(response) {
			lindex=(response['data']['index_limit']);
			document.getElementById('dindex').options.length = 0;
			var snd = document.getElementById('dindex');
			for (i = 0; i <= lindex; i++) {
				
				var opt = document.createElement('option');
				opt.innerHTML = i;
				opt.value = i;
				snd.appendChild(opt);
			}        
		}, function errorCallback(response) {
				alert('ERROR : Cannot get index from server !');
			});
	}
	
  $scope.show = function (){  
	var snd = document.getElementById('dindex');
	var cvalue=( snd.options[ snd.selectedIndex ].value );
	
	$http.get('http://127.0.0.1:80/api/blockchain/data?index='+cvalue, {}).then(function(response) {
		
        var chash=(response['data']['hash']);
		var phash=(response['data']['previous_hash']);
	
	    document.getElementById("current_hash").innerHTML ="Current Hash: "+ chash;
	    document.getElementById("previous_hash").innerHTML ="Previous Hash: "+phash;
    }, function errorCallback(response) {
            alert('ERROR : Cannot get blockchain data !');
  
  });
  }        
    $scope.add = function (){
        var data = $('#name').val()+$('#id').val()+$('#cgpa').val();
		
		var encodedString = (encodeURIComponent(btoa(data)));
		
        $scope.name1;
		$scope.pass1;
		
	
        $http.get('http://127.0.0.1:80/api/add?data='+encodedString, {}).then(function(response) {
        if(response['data']['status'] == 'true')
		{
			alert("Data added successfully. Your index is : " + response['data']['index']);
		}
		else
		{
			alert("ERROR : Cannot add data to the blockchain");
		}
        
    }, function errorCallback(response) {
            alert('ERROR : Cannot send data to add in blockchain');
  });
  clearInputFields();
    }
	$scope.verify = function (){
		var data = $('#name').val()+$('#id').val()+$('#cgpa').val();
		var indexsend=$('#index').val();

		var encodedString = (encodeURIComponent(btoa(data)));
		$http.get('http://127.0.0.1:80/api/verify?data='+encodedString+'&index='+indexsend, {}).then(function(response) {
        if(response['data']['status'] == 'true')
		{
			alert('Your data is valid.');
		}
		else
		{
			alert('ERROR : Data is invalid !');
		}
        clearInputFields();
    }, function errorCallback(response) {
            alert('ERROR : Cannot verify data from server !');
   
  });
    

	}


    
    
});