const moreBnt = document.querySelector('.info .metadata .moreBnt');
const title = document.querySelector('.info .metadata .title');

moreBnt.addEventListener('click',() => {
  moreBnt.classList.toggle('clicked');
  title.classList.toggle('clamp');
});
