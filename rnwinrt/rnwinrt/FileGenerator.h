#pragma once
#include "MetadataHelpers.h"
#include "Settings.h"

void WriteFiles(const Settings& settings, const std::map<std::string_view, std::shared_ptr<Namespace>>& roots);

void WriteFiles(const Settings& settings, const std::vector<std::shared_ptr<static_projection_data>>& roots);