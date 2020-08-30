$(document).ready(function()
{
    $("#servers_info_container").hide();
    $("#formBuscar").submit(function()
    {
        let servers_info_container = document.getElementById("servers_info_container");
        servers_info_container.innerHTML = "";
        $.ajax(
        {
            type: "POST",
            url: "/search",
            data:{ip_broadcast: $("#ip_broadcast").val()},
            success: function(respAx)
            {
                let respuesta = JSON.parse(respAx);
                let i;

                for(i = 0; i < respuesta.ips.length; i++)
                {
                    // console.log(respuesta.ips[i]);
                    let new_server = document.createElement("h6");
                    let new_server_text = document.createTextNode("IP: " + respuesta.ips[i] + " - Tiempo de respuesta(ms): " + respuesta.tiempos[i]);
                    new_server.appendChild(new_server_text);
                    servers_info_container.appendChild(new_server);
                }
                $("#servers_info_container").show();
            }
        });
        return false;
    });
});