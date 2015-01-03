var express = require('express');
var router = express.Router();
var User = require('../model/index.js');
var bodyParser = require('body-parser');

router.use(bodyParser.json());

router.use('/register', function(req, res, next) {
	if (req.ip == '127.0.0.1') {
		res.render('register.jade');
	}
	else {
		res.status(403).end()
	}

});

router.post('/UserRegister', function(req, res) {
	var registerInfo ={};
	registerInfo.account = req.body.account;
	registerInfo.password = req.body.password;
	registerInfo.email = req.body.email;
	registerInfo.type = req.body.type;

	User.createUser(registerInfo, function(retCode) {
	});
});


router.post('/login', function(req, res) {
	console.log('The req is:' + JSON.stringify(req.body))
	var loginUser = {};
	loginUser.account = req.body.account;
	loginUser.password = req.body.password;
	User.login(loginUser, function(retCode) {
		res.status(200).write(JSON.stringify(retCode));
		res.end();
	});
});

/* GET users listing. */
router.get('/', function(req, res) {
  res.send('respond with a resource');
});

module.exports = router;
