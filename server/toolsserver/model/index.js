var mongoose = require('mongoose');
mongoose.connect('mongodb://localhost/apktool');

var Schema = mongoose.Schema;

var User = new Schema({
	account:String,
	password:String,
	email:String,
	usertype:String

});

var loginUser = new Schema({
	account:String,
	password:String
});

var userModel = mongoose.model('User', User);
var userLoginModel = mongoose.model('loginUser', loginUser);

var db = {
	createUser : function(userinfo, cb) {
		console.log("The creatUser info:" + JSON.stringify(userinfo));
		var userRecord = new userModel(userinfo);
		userRecord.save(function(err) {
			console.log(err);
		});
	},

	login : function(userinfo, cb) {
		console.log("Login info is:" + userinfo.account + " " + userinfo.password);
		var userLoginRecord = new userLoginModel(userinfo);
		console.log("haha");
		userModel.findOne({account: userinfo.account}, function(err, doc) {
			if(err != null) {
				console.log(err);
				return;
			}
			else if( doc == null) {
				var returnCode = {};
				returnCode.code = 102;
				returnCode.msg = "user not found";
				cb(returnCode);
			}
			else if( doc != null) {
				if(userinfo.password == doc.password) {
					var returnCode = {};
					returnCode.code = 200;
					returnCode.msg = "login ok!";
					cb(returnCode);
				}
			}
		});

	}

};

module.exports = db;
