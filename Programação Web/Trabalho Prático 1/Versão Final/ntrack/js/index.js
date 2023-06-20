const pianoKeys = document.querySelectorAll(".key");

const audioFileInput = document.getElementById("arquivo");
const playButton = document.getElementById("play");
const pauseButton = document.getElementById("pause");
const stopButton = document.getElementById("stop");

const audioLength = document.getElementById("audio-length");
const currentTime = document.getElementById("current-time");

const progressBar = document.getElementById("progress-bar");
const bars = document.querySelectorAll(".bar");

const progressWave = document.querySelector(".wave-progress");
const waves = document.querySelectorAll(".wave-bar");

// Audio variables
let audio = new Audio();
let audioContext = new AudioContext();
let analyser = audioContext.createAnalyser();
analyser.fftSize = 2048;
let frequencyData = new Uint8Array(analyser.frequencyBinCount);

// Update the equalizer
function updateEqualizer() {
  requestAnimationFrame(updateEqualizer);
  analyser.getByteFrequencyData(frequencyData);
  bars.forEach((bar, i) => {
    const value = frequencyData[i];
    bar.style.height = value + "px";
  });
}

function update() {
  // Solicite que a função seja executada novamente na próxima atualização de quadro
  requestAnimationFrame(update);

  // Analise as frequências
  analyser.getByteFrequencyData(frequencyData);
  // Atualize a altura das barras com base na intensidade do som

  waves.forEach((wave, i) => {
    let barHeight = (frequencyData[i] / 255) * 60;
    wave.style.height = `${barHeight}px`;
  });
}

audio.addEventListener("canplaythrough", function () {
  console.log("can play");
  requestAnimationFrame(update);
});

// Change the audio
audioFileInput.addEventListener("change", function () {
  const files = this.files;
  if (files.length === 0) return;
  const file = files[0];
  const reader = new FileReader();
  reader.onload = function () {
    audio.src = reader.result;
    // Await the audio data to be ready
    audio.addEventListener("loadedmetadata", function () {
      const source = createMediaElementSource();
      source.connect(analyser);
      analyser.connect(audioContext.destination);
    });
  };
  reader.readAsDataURL(file);
  audioContext.resume();
});

// Create and AudioElementContext
function createMediaElementSource() {
  return audioContext.createMediaElementSource(audio);
}

// Play the audio
playButton.addEventListener("click", function () {
  playButton.classList.add("hidden");
  pauseButton.classList.remove("hidden");
  audio.play();
});

// Pause the audio
pauseButton.addEventListener("click", function () {
  pauseButton.classList.add("hidden");
  playButton.classList.remove("hidden");
  audio.pause();
});

// Stop the audio
stopButton.addEventListener("click", function () {
  audio.load();
  pauseButton.classList.add("hidden");
  playButton.classList.remove("hidden");
  progressBar.style.left = "0px";
});

// When the audio stop
audio.addEventListener("ended", function () {
  pauseButton.classList.add("hidden");
  playButton.classList.remove("hidden");
});

// Audio duration
audio.addEventListener("loadedmetadata", function () {
  const minutes = Math.floor(audio.duration / 60);
  const seconds = Math.floor(audio.duration % 60);
  audioLength.textContent = `- Duration ${minutes}:${seconds.toString().padStart(2, "0")}`;
});

audio.addEventListener("loadedmetadata", function () {
  analyser.connect(audioContext.destination);
  updateEqualizer();
});

// Audio current time
audio.addEventListener("timeupdate", function () {
  const minutes = Math.floor(audio.currentTime / 60);
  const seconds = Math.floor(audio.currentTime % 60);
  currentTime.textContent = `${minutes}:${seconds.toString().padStart(2, "0")}`;
});

// Progress
audio.addEventListener("timeupdate", function () {
  const progress = audio.currentTime / audio.duration;
  progressBar.style.left = progress * 100 + "%";
  if (progress * 100 == 100) {
    progressBar.style.left = "1px";
  }
});

// Progress
// audio.addEventListener("timeupdate", function () {
//   const progress = audio.currentTime / audio.duration;
//   progressWave.style.left = progress * 100 + "%";
//   if (progress == 1) {
//     progressWave.style.left = "0px";
//   }
// });

// Piano functions
function playKeySound(newUrl) {
  console.log(newUrl);
  new Audio(newUrl).play();
}

pianoKeys.forEach((pianoKey, i) => {
  const keys = [
    "02",
    "04",
    "06",
    "09",
    "11",
    "14",
    "16",
    "18",
    "21",
    "23",
    "01",
    "03",
    "05",
    "07",
    "08",
    "10",
    "12",
    "13",
    "15",
    "17",
    "19",
    "20",
    "22",
    "24",
  ];
  const newUrl = "assets/piano-sounds/key" + keys[i] + ".mp3";
  pianoKey.addEventListener("click", () => playKeySound(newUrl));
});


var mudarCorBotao = document.getElementById("botao");
var caixa = document.getElementsByClassName("caixa4")[0];

mudarCorBotao.addEventListener("click", function () {
  if (caixa.style.backgroundColor === "white") {
    caixa.style.backgroundColor = "black";
    caixa.style.color ="white";
  } else {
    caixa.style.backgroundColor = "white";
    caixa.style.color ="black";
  }
});