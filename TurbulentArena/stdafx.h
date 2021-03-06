// stdafx.h

#pragma once

#define NOMINMAX

//#pragma warning(disable:4099)
#pragma warning(disable: 4127 4005 4627)
#include "targetver.h"

#include "easylogging++.h"

#include <stdio.h>
#include <tchar.h>
#include <fstream>
#include <iostream>
#include <random>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <cassert>

// Additional headers

#include "sfml_include.hpp"
#include "box2d_include.hpp"
#include "tinyxml2.h"
#include "Utils.hpp"
#include "ServiceLocator.hpp"