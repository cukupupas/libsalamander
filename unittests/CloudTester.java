/*
Copyright 2016 Silent Circle, LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
import salamander.SalamanderNative;

public class CloudTester {

    static {
        System.loadLibrary("salamander++");
    }

    // typical chunk size
    static byte[] bigData = new byte[64*1024];
    static final String metadataBig = "This is bigger metadata";
    
    private static long cloudRef;

    public static void main(String[] args) {
        System.out.println("Hello, CloudTester!");

        try {
        
            byte[] metadata = metadataBig.getBytes("UTF-8");

            int[] code = new int[1];
            byte[] someRandom = {0,1,2,3,4,5,6,7,8,9};
            cloudRef = SalamanderNative.cloudEncryptNew(someRandom, bigData, metadata, code);

            if (code[0] != 0) {
                System.out.println("cloudEncryptNew code: " + code[0]);
                return;
            }
            System.out.println("cloudEncryptNew done.");
            
            code[0] = SalamanderNative.cloudCalculateKey(cloudRef);
            if (code[0] != 0) {
                System.out.println("cloudCalculateKey code: " + code[0]);
                return;
            }
            System.out.println("cloudCalculateKey done.");
            
            byte[] locatorRest = SalamanderNative.cloudEncryptGetLocatorREST(cloudRef, code);
            if (code[0] != 0) {
                System.out.println("cloudEncryptGetLocatorREST code: " + code[0]);
                return;
            }
            System.out.println("cloudEncryptGetLocatorREST done: " + new String(locatorRest));
            
            byte[] keyInfo = SalamanderNative.cloudEncryptGetKeyBLOB(cloudRef, code);
            if (code[0] != 0) {
                System.out.println("cloudEncryptGetKeyBLOB code: " + code[0]);
                return;
            }
            System.out.println("cloudEncryptGetKeyBLOB done: " + new String(keyInfo));

            byte[] segmentInfo = SalamanderNative.cloudEncryptGetSegmentBLOB(cloudRef, 1, code);
            if (code[0] != 0) {
                System.out.println("cloudEncryptGetSegmentBLOB code: " + code[0]);
                return;
            }
            System.out.println("cloudEncryptGetSegmentBLOB done: " + new String(segmentInfo));
            
            byte[] encryptedChunk = SalamanderNative.cloudEncryptNext(cloudRef, code);
            if (code[0] != 0) {
                System.out.println("cloudEncryptNext code: " + code[0]);
                return;
            }
            System.out.println("cloudEncryptNext done, length: " + encryptedChunk.length);

            // OK, release the context
            SalamanderNative.cloudFree(cloudRef);
            

            // Without randome data to re-hash the locator
            cloudRef = SalamanderNative.cloudEncryptNew(null, bigData, metadata, code);

            if (code[0] != 0) {
                System.out.println("cloudEncryptNew code: " + code[0]);
                return;
            }
            System.out.println("cloudEncryptNew done.");
            
            code[0] = SalamanderNative.cloudCalculateKey(cloudRef);
            if (code[0] != 0) {
                System.out.println("cloudCalculateKey code: " + code[0]);
                return;
            }
            System.out.println("cloudCalculateKey done.");
            
            locatorRest = SalamanderNative.cloudEncryptGetLocatorREST(cloudRef, code);
            if (code[0] != 0) {
                System.out.println("cloudEncryptGetLocatorREST code: " + code[0]);
                return;
            }
            System.out.println("cloudEncryptGetLocatorREST done: " + new String(locatorRest));
            
            keyInfo = SalamanderNative.cloudEncryptGetKeyBLOB(cloudRef, code);
            if (code[0] != 0) {
                System.out.println("cloudEncryptGetKeyBLOB code: " + code[0]);
                return;
            }
            System.out.println("cloudEncryptGetKeyBLOB done: " + new String(keyInfo));

            segmentInfo = SalamanderNative.cloudEncryptGetSegmentBLOB(cloudRef, 1, code);
            if (code[0] != 0) {
                System.out.println("cloudEncryptGetSegmentBLOB code: " + code[0]);
                return;
            }
            System.out.println("cloudEncryptGetSegmentBLOB done: " + new String(segmentInfo));
            
            encryptedChunk = SalamanderNative.cloudEncryptNext(cloudRef, code);
            if (code[0] != 0) {
                System.out.println("cloudEncryptNext code: " + code[0]);
                return;
            }
            System.out.println("cloudEncryptNext done, length: " + encryptedChunk.length);

            // OK, release the context
            SalamanderNative.cloudFree(cloudRef);


            /* ****  Decrypt the data  */
            
            cloudRef = SalamanderNative.cloudDecryptNew (keyInfo, code);
            if (code[0] != 0) {
                System.out.println("cloudDecryptNew code: " + code[0]);
                return;
            }
            System.out.println("cloudDecryptNew done.");
            
            code[0] = SalamanderNative.cloudDecryptNext(cloudRef, encryptedChunk);
            if (code[0] != 0) {
                System.out.println("cloudDecryptNext code: " + code[0]);
                return;
            }
            System.out.println("cloudDecryptNext done.");
            
            byte[] decryptedData = SalamanderNative.cloudGetDecryptedData(cloudRef);
            System.out.println("cloudGetDecryptedData done, length: " + decryptedData.length);

            byte[] decryptedMetaData = SalamanderNative.cloudGetDecryptedMetaData(cloudRef);
            System.out.println("cloudGetDecryptedMetaData done: " + new String(decryptedMetaData));

            // OK, release the context
            SalamanderNative.cloudFree(cloudRef);
        }
        catch (Exception ex) {
            System.out.println("Got an exception: " + ex);
        }
    }
}
