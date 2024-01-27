// Function to give recommendations based on soil moisture content
function recommendPlantCareSoil(moisture) {
    if (moisture < 30) {
        return "Water the plant";
    } else if (moisture >= 30 && moisture <= 60) {
        return "Soil moisture is optimal";
    } else {
        return "Reduce watering";
    }
}

// Function to give recommendations based on ambient light levels
function recommendPlantCareLight(light) {
    if (light < 500) {
        return "Move the plant to a brighter location";
    } else if (light >= 500 && light <= 2000) {
        return "Light levels are optimal";
    } else {
        return "Reduce exposure to direct sunlight";
    }
}

// Function to give recommendations based on temperature
function recommendPlantCareTemperature(temperature) {
    if (temperature < 15) {
        return "Move the plant to a warmer location";
    } else if (temperature >= 15 && temperature <= 30) {
        return "Temperature is optimal";
    } else {
        return "Provide shade or cooling for the plant";
    }
}
