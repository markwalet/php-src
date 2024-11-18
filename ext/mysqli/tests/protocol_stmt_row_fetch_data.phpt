--TEST--
MySQL protocol - statement row data fetch)
--EXTENSIONS--
mysqli
--FILE--
<?php
require_once 'fake_server.inc';

$port = 33305;
$servername = "127.0.0.1";
$username = "root";
$password = "";

$process = run_fake_server_in_background('stmt_response_row_read_two_fields', $port);
$process->wait();

$conn = new mysqli($servername, $username, $password, "", $port);

function my_query($conn, $field)
{
    $stmt = $conn->prepare("SELECT strval, $field FROM data");

    $stmt->execute();
    $result = $stmt->get_result();

    if ($result->num_rows > 0) {
        while ($row = $result->fetch_assoc()) {
            var_dump($row[$field]);
        }
    }
}

foreach (my_mysqli_data_fields() as $field_name => $field) {
    my_query($conn, $field_name);
}

$conn->close();

$process->terminate(true);

print "done!";
?>
--EXPECTF--
[*] Server started
[*] Connection established
[*] Sending - Server Greeting: 580000000a352e352e352d31302e352e31382d4d6172696144420003000000473e3f6047257c6700fef7080200ff81150000000000000f0000006c6b55463f49335f686c6431006d7973716c5f6e61746976655f70617373776f7264
[*] Received: 6900000185a21a00000000c0080000000000000000000000000000000000000000000000726f6f7400006d7973716c5f6e61746976655f70617373776f7264002c0c5f636c69656e745f6e616d65076d7973716c6e640c5f7365727665725f686f7374093132372e302e302e31
[*] Sending - Server OK: 0700000200000002000000
[*] Received: 200000001653454c4543542073747276616c2c20696e7476616c2046524f4d2064617461
[*] Sending - Stmt prepare data intval: 0c0000010001000000020000000000003200000203646566087068705f74657374046461746104646174610673747276616c0673747276616c0ce000c8000000fd01100000003200000303646566087068705f746573740464617461046461746106696e7476616c06696e7476616c0c3f000b00000003011000000005000004fe00000200
[*] Received: 0a00000017010000000001000000
[*] Sending - Stmt execute data intval: 01000001023200000203646566087068705f74657374046461746104646174610673747276616c0673747276616c0ce000c8000000fd01100000003200000303646566087068705f746573740464617461046461746106696e7476616c06696e7476616c0c3f000b00000003011000000005000004fe000022000b000005000004746573740e00000005000006fe00002200
int(14)
[*] Received: 050000001901000000200000001653454c4543542073747276616c2c20666c7476616c2046524f4d2064617461
[*] Sending - Stmt prepare data fltval: 0c0000010001000000020000000000003200000203646566087068705f74657374046461746104646174610673747276616c0673747276616c0ce000c8000000fd01100000003200000303646566087068705f746573740464617461046461746106666c7476616c06666c7476616c0c3f000c0000000401101f000005000004fe00000200
[*] Received: 0a00000017010000000001000000
[*] Sending - Stmt execute data fltval: 01000001023200000203646566087068705f74657374046461746104646174610673747276616c0673747276616c0ce000c8000000fd01100000003200000303646566087068705f746573740464617461046461746106666c7476616c06666c7476616c0c3f000c0000000401101f000005000004fe000022000b000005000004746573743333134005000006fe00002200
float(2.3)
[*] Received: 050000001901000000200000001653454c4543542073747276616c2c2064626c76616c2046524f4d2064617461
[*] Sending - Stmt prepare data dblval: 0c0000010001000000020000000000003200000203646566087068705f74657374046461746104646174610673747276616c0673747276616c0ce000c8000000fd01100000003200000303646566087068705f74657374046461746104646174610664626c76616c0664626c76616c0c3f00160000000501101f000005000004fe00000200
[*] Received: 0a00000017010000000001000000
[*] Sending - Stmt execute data dblval: 01000001023200000203646566087068705f74657374046461746104646174610673747276616c0673747276616c0ce000c8000000fd01100000003200000303646566087068705f74657374046461746104646174610664626c76616c0664626c76616c0c3f00160000000501101f000005000004fe000022000f00000500000474657374333333333333f33f05000006fe00002200
float(1.2)
[*] Received: 050000001901000000200000001653454c4543542073747276616c2c2064617476616c2046524f4d2064617461
[*] Sending - Stmt prepare data datval: 0c0000010001000000020000000000003200000203646566087068705f74657374046461746104646174610673747276616c0673747276616c0ce000c8000000fd01100000003200000303646566087068705f74657374046461746104646174610664617476616c0664617476616c0c3f000a0000000a811000000005000004fe00000200
[*] Received: 0a00000017010000000001000000
[*] Sending - Stmt execute data datval: 01000001023200000203646566087068705f74657374046461746104646174610673747276616c0673747276616c0ce000c8000000fd01100000003200000303646566087068705f74657374046461746104646174610664617476616c0664617476616c0c3f000a0000000a811000000005000004fe000022000c0000050000047465737404de070c0f05000006fe00002200
string(10) "2014-12-15"
[*] Received: 050000001901000000200000001653454c4543542073747276616c2c2074696d76616c2046524f4d2064617461
[*] Sending - Stmt prepare data timval: 0c0000010001000000020000000000003200000203646566087068705f74657374046461746104646174610673747276616c0673747276616c0ce000c8000000fd01100000003200000303646566087068705f74657374046461746104646174610674696d76616c0674696d76616c0c3f000a0000000b811000000005000004fe00000200
[*] Received: 0a00000017010000000001000000
[*] Sending - Stmt execute data timval: 01000001023200000203646566087068705f74657374046461746104646174610673747276616c0673747276616c0ce000c8000000fd01100000003200000303646566087068705f74657374046461746104646174610674696d76616c0674696d76616c0c3f000a0000000b811000000005000004fe00002200100000050000047465737408000000000015080105000006fe00002200
string(8) "21:08:01"
[*] Received: 050000001901000000200000001653454c4543542073747276616c2c2064746976616c2046524f4d2064617461
[*] Sending - Stmt prepare data dtival: 0c0000010001000000020000000000003200000203646566087068705f74657374046461746104646174610673747276616c0673747276616c0ce000c8000000fd01100000003200000303646566087068705f74657374046461746104646174610664746976616c0664746976616c0c3f00130000000c811000000005000004fe00000200
[*] Received: 0a00000017010000000001000000
[*] Sending - Stmt execute data dtival: 01000001023200000203646566087068705f74657374046461746104646174610673747276616c0673747276616c0ce000c8000000fd01100000003200000303646566087068705f74657374046461746104646174610664746976616c0664746976616c0c3f00130000000c811000000005000004fe000022000f0000050000047465737407de070c100d000105000006fe00002200
string(19) "2014-12-16 13:00:01"
[*] Received: 050000001901000000200000001653454c4543542073747276616c2c2062697476616c2046524f4d2064617461
[*] Sending - Stmt prepare data bitval: 0c0000010001000000020000000000003200000203646566087068705f74657374046461746104646174610673747276616c0673747276616c0ce000c8000000fd01100000003200000303646566087068705f74657374046461746104646174610662697476616c0662697476616c0c3f004000000010211000000005000004fe00000200
[*] Received: 0a00000017010000000001000000
[*] Sending - Stmt execute data bitval: 01000001023200000203646566087068705f74657374046461746104646174610673747276616c0673747276616c0ce000c8000000fd01100000003200000303646566087068705f74657374046461746104646174610662697476616c0662697476616c0c3f004000000010211000000005000004fe00002200100000050000047465737408080808080808080805000006fe00002200
%s578721382704613384%s
[*] Received: 050000001901000000200000001653454c4543542073747276616c2c2073747276616c2046524f4d2064617461
[*] Sending - Stmt prepare data strval: 0c0000010001000000020000000000003200000203646566087068705f74657374046461746104646174610673747276616c0673747276616c0ce000c8000000fd01100000003200000303646566087068705f74657374046461746104646174610673747276616c0673747276616c0ce000c8000000fd011000000005000004fe00000200
[*] Received: 0a00000017010000000001000000
[*] Sending - Stmt execute data strval: 01000001023200000203646566087068705f74657374046461746104646174610673747276616c0673747276616c0ce000c8000000fd01100000003200000303646566087068705f74657374046461746104646174610673747276616c0673747276616c0ce000c8000000fd011000000005000004fe000022000c00000500000474657374047465737405000006fe00002200
string(4) "test"
[*] Received: 0500000019010000000100000001
[*] Server finished
done!
