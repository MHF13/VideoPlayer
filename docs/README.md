# Video Player

# Links
- [Vfw.h documentation](https://docs.microsoft.com/en-us/windows/win32/api/vfw/)
- [Handout](https://docs.microsoft.com/en-us/windows/win32/api/vfw/) X
- [Presentation](https://docs.microsoft.com/en-us/windows/win32/api/vfw/) X
- [Solution](https://github.com/MHF13/VideoPlayer/tree/main/Solution) 
- [Release](https://docs.microsoft.com/en-us/windows/win32/api/vfw/) X

# What has been used? 

## State machine
En el Update de 'VideoPlayer' podremos ver una maquina de estados que se encarga de la reproduccion y finalizacion del video.
- [START] Reporiduce el audio del video y accede al primer frame.
- [PLAY] Accedera a los frames a medida que el video avance hasta el final de este.
- [FINISH] Llamara a la funcion que desactivara el reproductor de video.


# TODOs
Si descargáis la Release y abrís el proyecto Hangouts podréis, siguiendo estos TODOs implementar un video player en un pequeño proyecto.

## TODO 0
Antes de comencar hay que descomprimir el video para que pueda ser leido por nuestro programa.
Dirigios a \Output\Assets\Video y descomprimid el archivo llamado video1.zip, es importante que el video y el audio que encontrareis tengan el mismo nombre.

![TODO_0](images/eru's_adventures.gif)

## TODO 1
Empezaremos llamando al reproductor de video en el momento en el que queremos que se inicie, en este caso en el Start de Scene llamando a la funcion StartVideo que devolvera un booleano. 
Lo igualaremos a la variable videoActive, esta nos indicara que se ha iniciado la reproduccion de un video.
Es importante que el path este sin formato para que pueda ser usado tanto para el audio como para el video mas adelante.
```
bool Scene::Start()
{
  ...
  // TODO 1: Play video
  videoActive = app->video->StartVideo("Assets/Video/video1");
  ...
}
```
### TODO 1.1
Para acceder a los archivos de audio y video capiaremos el path en 2 char* y añadiremos la extension correspondiente a cada uno de ellos.
```
bool VideoPlayer::StartVideo(char* filePath)
{
  ...
  // TODO 1.1: We make the paths for the video and audio files from the filePath 
  int size = strlen(filePath);
  int nieuwSize = size + 5;
  audio = (char*)malloc(nieuwSize);
  strcpy_s(audio, nieuwSize, path.GetString());
  strcat_s(audio, nieuwSize, ".ogg");

  video = (char*)malloc(nieuwSize);
  strcpy_s(video, nieuwSize, path.GetString());
  strcat_s(video, nieuwSize, ".avi");
  ...
}
```
## TODO 2
Inizializaremos la libreria AVIFile y accederemos al archivo AVI y a sus datos
```
bool VideoPlayer::StartVideo(char* filePath)
{
  ...
  // TODO 2: Activate the library and access the video information 

  // The AVIFileInit function initializes the AVIFile library.
  AVIFileInit();

  // Open input AVI file
  AVIFileOpenA(&fileAVI, (LPCSTR)video, OF_SHARE_DENY_WRITE, NULL);

  // Get file info
  AVIFileInfo(fileAVI, &fileInfo, sizeof(AVIFILEINFO));
  // Save width and height of video
  width = fileInfo.dwWidth;
  height = fileInfo.dwHeight;

  // Get video stream
  AVIFileGetStream(fileAVI, &fileStream, streamtypeVIDEO, 0);
  if (fileStream == NULL)
  	LOG("Failed To Open The AVI Stream");
  ...
}
```
## TODO 3
Para que nuestro programa se sincronize con los FPS del video obtendremos su framarete con unas operaciones sencillas y cambiaremos los FPS del programa al del video.
A su vez, guardaremos como variable el framerate que tenia anteriormente nuestro programa para devolverselo al finalizar el video.
```
bool VideoPlayer::StartVideo(char* filePath)
{
  ...
  // TODO 3: Set the program to the framerate of the video 
  nFrames = AVIStreamLength(fileStream); // Video frames  
  timeVideo = AVIStreamLengthTime(fileStream) / 1000; // Video time in seconds 

  FPS = nFrames / timeVideo; // fps of the video 
  prevFPS = app->GetFramerate(); // Save actual framerate
  app->ChangeFPS(FPS); // Change FPS
  ...
}
```
### TODO 3.1
Una vez que la reproducción haya finalizado deberemos restaurar el framerate del programa
```
bool VideoPlayer::CleanUp()
{
  ...
  // TODO 3.1: Restore FPS 
  app->ChangeFPS(prevFPS);
  ...
}
```
## TODO 4
Descomprimiremos los fotogramas del stream del video y los almacenaremos en frmSequence

```
bool VideoPlayer::StartVideo(char* filePath)
{
  ...
  /* 
  TODO 4: Decompressing the frames of the video stream 
  This function returns a PGETFRAME
  We will be able to visualize individual frames of an open sequence.
  */
  frmSequence = AVIStreamGetFrameOpen(fileStream, NULL);
  if (frmSequence == NULL)
  	LOG("Failed To Open The AVI Frame");
  ...
}
```
## TODO 5
Para poder obtener un frame del video para dibujarlo en pantalla deberemos seguir los siguientes pasos:
1. Crear un bitmap del frame deseado
2. Almacenar un puntero a bitmapData
3. Obtener una surface del frame deseado
4. Crear una textura a partir de la surface
Al final de esta funcion aumentaremos el indice para que posteriormente se obtenga el siguiente frame.
```
void VideoPlayer::NextFrame()
{
  // TODO 5: Create a texture of the current frame 
  // Create a bitmap for select frame
  LPBITMAPINFOHEADER bitmap = NULL;
  bitmap = (LPBITMAPINFOHEADER)AVIStreamGetFrame(frmSequence, frameIndex);
  
  // Pointer to bitmap data
  pointerData = (char*)bitmap + bitmap->biSize + bitmap->biClrUsed * sizeof(RGBQUAD); 

  // Create a surface of the current frame
  surface = SDL_CreateRGBSurfaceFrom(pointerData, width, height, bitmap->biBitCount, width * 3, 0, 0, 0, 0);
  
  // Get the texture 
  textureFrame = app->tex->LoadSurface(surface);

  // Next frame
  frameIndex++;
}
```
## TODO 6
En el primer estado de la mquina de estados haremos que el audio se reproduzca.

```
bool VideoPlayer::Update(float dt)
  ...
  {
    // TODO 6: Play music
    app->audio->PlayMusic(audio);
    ...
  }
  ...
```
### TODO 6.1
Tambien deberemos detener la musica una vez finalizada la reproduccion
```
bool VideoPlayer::CleanUp()
{
  ...
  // TODO 6.1: Stop the music 
  Mix_HaltMusic();
  ...
}
```
## TODO 7
Pintaremos la textura del frame actual pero solo se hara durante el estado de PLAY
It is important that both the texture and the surface are released to avoid memory leaks 
```
bool VideoPlayer::PostUpdate()
{
  if (state == PLAY)
  {
    // TODO 7: Draw the texture of the frame.(180 angle because the image obtained is rotated)
    app->render->DrawTexture(textureFrame, (WINDOW_W / 2) - (width / 2), (WINDOW_H / 2) - (height / 2), NULL, 0, 180);
  
    // TODO 7:To avoid memory overload, we will UnLoad the texture and free the surface after each frame is painted. 
    app->tex->UnLoad(textureFrame);
    SDL_FreeSurface(surface);	
  }
  return true;
}
```
### TODO 7.1
Tambien debemos liberar la textura y la surface cunado finalize la reproduccion
```
bool VideoPlayer::CleanUp()
{
  ...
    // TODO 7.1:Remember to unload the texture and free the surface here too.
    app->tex->UnLoad(textureFrame);
    SDL_FreeSurface(surface);
  ...
}
```

# TODO BONUS




You can use the [editor on GitHub](https://github.com/MHF13/VideoPlayer/edit/gh-pages/index.md) to maintain and preview the content for your website in Markdown files.

Whenever you commit to this repository, GitHub Pages will run [Jekyll](https://jekyllrb.com/) to rebuild the pages in your site, from the content in your Markdown files.

### Markdown

Markdown is a lightweight and easy-to-use syntax for styling your writing. It includes conventions for


Syntax highlighted code block

```markdown
# Header 1
## Header 2
### Header 3
```
# Header 1
## Header 2
### Header 3

- Bulleted
- List

1. Numbered
2. List

**Bold** and _Italic_ and `Code` text

[Link](url) and ![Image](src)

For more details see [GitHub Flavored Markdown](https://guides.github.com/features/mastering-markdown/).

### Jekyll Themes

Your Pages site will use the layout and styles from the Jekyll theme you have selected in your [repository settings](https://github.com/MHF13/VideoPlayer/settings/pages). The name of this theme is saved in the Jekyll `_config.yml` configuration file.

### Support or Contact

Having trouble with Pages? Check out our [documentation](https://docs.github.com/categories/github-pages-basics/) or [contact support](https://support.github.com/contact) and we’ll help you sort it out.
