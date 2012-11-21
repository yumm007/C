const char *open_str="<stream:stream to=\"192.168.1.127\" xmlns=\"jabber:client\" xmlns:stream=\"http://etherx.jabber.org/streams\" version=\"1.0\">";
const char *register_str="<iq type='get' to='192.168.1.127' id='auth1'> <query xmlns='jabber:iq:auth'/> </iq>";
const char *register_ack="<iq type='set' id='auth2'><query xmlns='jabber:iq:auth'><username>211</username><password>211</password><resource>globe</resource></query></iq>";
const char *online="<presence><status>Online</status></presence>";

