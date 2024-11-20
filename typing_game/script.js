var q = ["hokkaido","aomori", "akita", "iwate", "yamagata", "miyagi", "fukushima", 
  "niigata", "toyama", "ishikawa", "fukui", "nagano", "gunma", "tochigi", 
  "ibaraki", "chiba", "saitama", "tokyo", "kanagawa", "yamanashi", "shizuoka", 
  "aichi", "gifu", "mie", "shiga", "kyoto", "osaka", "nara", "wakayama", "hyogo", 
  "tottori", "shimane", "okayama", "hiroshima", "yamaguchi", "kagawa", "tokushima", 
  "koochi", "ehime", "fukuoka", "oita", "miyazaki", "kumamoto", "kagoshima", "saga", 
  "nagasaki", "okinawa"];

var q_i = 0;
var q_no = Math.floor(Math.random() * q.length);
var q_l = q[q_no].length;
var timeLimit = 30;

var dispScore = document.getElementById('score');
var dispTimer = document.getElementById('timer');
var question = document.getElementById('start');

var check = [];

function create() {
  q_no = Math.floor(Math.random() * q.length);

  check = q[q_no].split('');
  question.textContent = check.join('');
}

var startTime, endTime, score = 0;

function start() {
  score = 0;
  startTime = Date.now();
  endTime = startTime + (timeLimit * 1000);
  updatescore();
  timer();

  create();
}

function updatescore() {
  dispScore.textContent = 'スコア：' + score;
}

function timer() {
  var current = Date.now();
  var remaining = Math.floor((Math.max(0, endTime - current)) / 1000);

  dispTimer.textContent = '残り' + remaining + '秒';

  if(remaining > 0) {
    requestAnimationFrame(timer);
  } else {
    dispTimer.textContent = '終わり！スペースキーを押すともう一度遊べるよ！';
    question.textContent = '';
    flag = 0;
  }
}

let flag = 0;

document.addEventListener('keydown', function(e) {
  if(flag === 1 && e.key === check[0]) {
    check.shift();
    question.textContent = check.join('');

    if(!check.length) {
      create();
      score++;
      updatescore();
    }
  } else if(flag == 0 && e.code === "Space") {
    start()
    flag = 1;
  }
});

