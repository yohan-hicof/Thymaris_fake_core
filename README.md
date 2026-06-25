This software is a fake one. The goal is to imitate the behavior of Thymaris_core to test the Thymaris Cellphone app.

This software is called the same way the Thymaris core software is called when we want to compare an image.
Then it read the signature, and answer accordingly. It mimics Thymaris Core software without actually processing anything.

It opens a fixed json file: Thymaris_fake_core.json, supposed to be in the same folder and adapt its behavior according to this json.

This software also update the local DB to avoid having this code blacklisted due to their expected high number of scans.


The normal call of the Thymaris core software:`DM_find_i_R_0.85 -C -i /path/to/image.png -s SIGNATURE`. This software can take the same
call, however, it will only looks for the `-s SIGNATURE` part and discards the rest.

When called with `-add`, it will add/update to the `signature` table the serialnumber/signature found in the json file.

The two main DB queries are:

`INSERT INTO signatures (serialnumber, signature) VALUES ($1::text, $2::text) ON CONFLICT (serialnumber) DO UPDATE SET signature = EXCLUDED.signature;`

`UPDATE request_history SET metadata=(metadata::jsonb || '{\"scan_count\":0}'::jsonb) WHERE identifier IN (xxx, yyy)";`


Expected output:
  - Date time (cerr)
  - Message: Extract from given signature (cerr)
  - DM area: use the image corner (cerr)
  - |Result|X|: (cout)
  - exit(X)


Content of the json:
```
{
    "contents": [
        {"I_FUNCTION_CHECK:VALID": "HICOF_ORIGINAL"},
        {"I_FUNCTION_CHECK:INVALID": "HICOF_COUNTERFEIT"},
        {"I_FUNCTION_CHECK:NULL": "HICOF_UNSURE"},
        {"L_FUNCTION_CHECK:VALID": "HICOF_ORIGINAL"},
        {"L_FUNCTION_CHECK:INVALID": "HICOF_COUNTERFEIT"},
        {"L_FUNCTION_CHECK:NULL": "HICOF_UNSURE"}
    ],
    "postgres_login": "postgres",
    "postgres_password": "postgres",
    "postgres_database": "postgres",
    "postgres_ip": "127.0.0.1",
    "postgres_port": "5432"
}
```
---
Here is the list of Base64 signatures:
```
Zl9jXzAuMDBJX0ZVTkNUSU9OX0NIRUNLLVZBTElE
Zl9jXzAuMDBJX0ZVTkNUSU9OX0NIRUNLOklOVkFMSUQ=
Zl9jXzAuMDBJX0ZVTkNUSU9OX0NIRUNLOk5VTEw=
Zl9jXzAuMDBMX0ZVTkNUSU9OX0NIRUNLLVZBTElE
Zl9jXzAuMDBMX0ZVTkNUSU9OX0NIRUNLOklOVkFMSUQ=
Zl9jXzAuMDBMX0ZVTkNUSU9OX0NIRUNLOk5VTEw=
```
---


Example of the output of the normal DM find:
b'Date: May 26 2026, Time: 10:18:07
Message: L9VZ5Y46P7
Printed ratio : 0.796577
[111.182, 299.231][298.266, 306.946][303.949, 116.299][116.14, 109.142]
Error in function : compare_signature_inkjet_ghana
The quality of the given image is below minimal standard
Warning, I deactivated the quality reject for testing purpose
Warning from function sufficient_image_resolution
It seems that the image given to compare to the signature does not contains enough pixels per module
The final result may not reflect the real origin of the code
M52 differences: 1822.5
Printed ratio : 0.796577
WARNING SPECIAL TEMPLATE MODIFICATION FOR 14X14 MODULES SIZE
|LEFTLINEVALUE|14063.8|
|RIGHTLINEVALUE|12566.5|
|LINEVALUE|14063.8|
|MEANSPIKEVALUE|17787|
--- M28 Score: 0.989714--- sum bars: 0 --- sum spikes: 0.0171429
The file ./spikes_photocopy.onnx cannot be found, not testing photocopy.
Number of comparison 8
M30: Total: 8, Distance: 0.25, score: 0.96875
----------------------------2.2392-----------------------------
The file ./recapture_classifier.onnx cannot be found, not testing for recapture.
Could not load the classifier model, recapture not evaluated
liste scores : [52,0.95] [9,0.421053] [16,7950.98] [28,0.989714] [30,0.96875]
|SCORES|52:0.95|9:0.421053|16:7950.98|28:0.989714|30:0.96875|
Sum : 1.64455 from : [4.5,0,0,0]
'
---------COUT---------
b'|Result|1|
Computation time: 161ms