# tpc

**!! art by https://x.com/slugtipede !!**  
5 pngs that change depending on mouse and keyboard input (+ backspace), customizable if PNGs have 11:10 aspect ratio  
made for my gf's horrible macbook  

## macOS (setup required): 
move tpc.app into Applications

paste `xattr -dr com.apple.quarantine /Applications/tpc.app` in terminal  

give tpc accessibility perms

## Windows:
run tpc.exe

### odd behaviors but not bugs:
- doesn't work with exclusive fullscreen
- doesn't work with anti-cheats that prevent global input reading
- can sit under the taskbar if certain actions happen in a certain order (Windows)
- always sits under the task bar (in macOS)

### bugs:
- black background of PNG on some Windows system (not sure why yet)

### plans:
- make macOS out of box ready
- easier png customization (remove the strict 11:10px png requirement)
- divide left and right by left and right of qwerty
- add !, ?, -(?)
