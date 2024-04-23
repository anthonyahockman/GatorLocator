const uf = { lat: 29.646534033554595, lng: -82.35331855744083 };  // uf coordinates (temporary)
let map;
let directionsService;
let directionsRenderer;

function initMap() 
{
    directionsService = new google.maps.DirectionsService();
    directionsRenderer = new google.maps.DirectionsRenderer();
    // initialize the map
    map = new google.maps.Map(document.getElementById("map"), {
        zoom: 18,  // zoom level (higher zoom = more detail)
        center: uf,  // location of the center of the map
        scrollwheel: true,  // users can zoom in and out using mouse scroll
        clickableIcons: true,  // users can click on markers + icons on map
    });
    // set a marker at UF
    const marker = new google.maps.Marker({
        position: uf,
        map: map,
        title: "UF",
        icon: "Gator marker.webp",
    });
    directionsRenderer.setMap(map);
}

function calculatePath() 
{
    const searchQuery = document.getElementById("searchInput").value;  // get user address from search bar
    const geocoder = new google.maps.Geocoder();
    geocoder.geocode({ address: searchQuery }, function (results, status) {
        if (status == google.maps.GeocoderStatus.OK) 
        {
            // if address is valid, then...
            //let start_pos = { lat: results[0].geometry.location.lat(), lng: results[0].geometry.location.lng() };  // set the starting position
            // random array of points (these would be the points returned from the algorithm)
            const points = [{ lat: 25.7798729, lng: -80.1511307}]; // array of intersections returned by algorithms
            let start_pos = { lat: 25.7800522, lng: -80.1514359};
            const waypts = [];  // array for waypoints (intersections between origin and destination)
            for (let i = 0; i < points.length; i++) {
                waypts.push({
                    location: points[i],
                    stopover: true
                });
            }
            // set route
            directionsService.route({
                origin: start_pos,
                destination: uf,
                waypoints: waypts,
                optimizeWaypoints: true,
                travelMode: google.maps.TravelMode.DRIVING,
            })
                .then((response) => {
                    directionsRenderer.setDirections(response);
                    // erases error message
                    document.getElementById("searchResult").innerText = "";
                })
        } 
        else 
        {
          document.getElementById("searchResult").innerText = "Error, invalid address";  // displays error message
        }
    });
}

// event handling (when user clicks the search icon)
document.getElementById("searchForm").addEventListener("submit", function (event) 
{
    event.preventDefault();
    calculatePath();
});

// initialize the map
window.initMap = initMap;