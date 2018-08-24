## Note on using Arduino MKR1000 as Client

When using MKR1000 as the client to subscribe to the channel(s), run the lab3-server-slow.js file. This is to ensure that the publish/subscribe rates are within the soft-limits. The reason being the fact that PubNub SDK for Arduino MKR1000 is quite small and modest.

## Using the Web Browser as Client

In this case, run the lab3-server.js along with lab3-client-visuals.html for the full experience.