# a-star-cities-2

Visualisation of A* path finding algorithm using open street map data.

The map data can be downloaded via the [open street map API](https://wiki.openstreetmap.org/wiki/Downloading_data).
The data must then be saved as .json file to be used by the program.

Better and faster implementation of an earlier project: [GitHub](https://github.com/Lewingston/a-star-cities-2)

Inspired by this [video](https://www.youtube.com/watch?v=CgW0HPHqFE8).

## Usage

Pass the json file, containing the map data, to the program via command line parameter.

```
astarcities2.exe mapdata.json
```

There is an optional interactive mode, which allows to change some parameters of the animation:

```
astarcities2.exe interactive mapdata.json
```

## Demo

![Demo](docs/new_york_demo.gif)