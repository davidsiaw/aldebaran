local greyfloor = 
{
	id = 1,
	path = "tiles/floors.png",
	x = 0,
	y = 0,
	w = 32,
	h = 32
}

local brownfloor = 
{
	id = 2,
	path = "tiles/floors.png",
	x = 32,
	y = 0,
	w = 32,
	h = 32
}

local wing1 =
{
	id = 3,
	path = "sprites/wingman.png",
	x = 0,
	y = 0,
	w = 48,
	h = 48
}


local wing2 =
{
	id = 4,
	path = "sprites/wingman.png",
	x = 48,
	y = 0,
	w = 48,
	h = 48
}

local wing3 =
{
	id = 5,
	path = "sprites/wingman.png",
	x = 96,
	y = 0,
	w = 48,
	h = 48
}

local wing4 =
{
	id = 6,
	path = "sprites/wingman.png",
	x = 144,
	y = 0,
	w = 48,
	h = 48
}

local pokemon =
{
	id = 7,
	path = "pokemon.png",
	x = 0,
	y = 0,
	w = 192,
	h = 192
}

local arrow =
{
	id = 8,
	path = "images/arrow.png",
	x = 0,
	y = 0,
	w = 300,
	h = 294
}

-- sprites

local wing =
{
	id = 10001,
	imageIds = {3,4,5,6},
	delay = 200
}

-- font

local helvetica = 
{
	id = 20001,
	path = "fonts/HelveticaBold.ttf",
	color = { r=0xff, g=0xff, b=0xff },
	ptsize = 50
}

local helveticaLight =
{
	id = 20002,
	path = "fonts/HelveticaLT45Light.ttf",
	color = { r=0xff, g=0xff, b=0xff },
	ptsize = 50
}

local sazanami =
{
	id = 20003,
	path = "fonts/sazanami-gothic.ttf",
	color = { r=0xff, g=0xff, b=0xff },
	ptsize = 50
}

-- decls

images =
{
	greyfloor,
	brownfloor,
	wing1,
	wing2,
	wing3,
	wing4,
	pokemon,
	arrow
}

sprites =
{
	wing
}

fonts =
{
	helvetica,
	helveticaLight,
	sazanami
}
