import cv2.aruco as aruco
import cv2
import numpy as np

url = 'https://192.168.0.3:8080/video'
cap = cv2.VideoCapture(url)

aruco_dict = aruco.Dictionary_get(aruco.DICT_6X6_250)
parameters = aruco.DetectorParameters_create()

image = cv2.imread("22.jpg")

while True:
    _, img = cap.read()
    corners, ids, _ = aruco.detectMarkers(img, aruco_dict, parameters=parameters)

    if corners is not None and len(corners) >= 4:

        top_left = None
        top_right = None
        bottom_right = None
        bottom_left = None

        for i, corner in enumerate(corners):
            x = corner[0][0][0]
            y = corner[0][0][1]
            height, width = img.shape[:2]


            if x < width // 2 and y < height // 2:
                top_left = (x, y)
            elif x > width // 2 and y < height // 2:
                top_right = (x, y)
            elif x > width // 2 and y > height // 2:
                bottom_right = (x, y)
            elif x < width // 2 and y > height // 2:
                bottom_left = (x, y)

        if top_left is not None and top_right is not None and bottom_right is not None and bottom_left is not None:
            points = np.array([top_left, top_right, bottom_right, bottom_left], dtype=np.int32)

            pts1 = np.array([top_left, top_right, bottom_right, bottom_left])
            pts2 = np.float32([[0, 0], [width, 0], [width, height], [0, height]])

            resizeimg = cv2.resize(image, (img.shape[1], img.shape[0]))

            matris, _ = cv2.findHomography(pts2, pts1)
            imgOut = cv2.warpPerspective(resizeimg, matris, (img.shape[1], img.shape[0]))
            cv2.fillConvexPoly(img, pts1.astype(int), (0, 0, 0))

            img = cv2.add(imgOut, img)

            cv2.polylines(img, [points], True, (0, 255, 0), 2)

    cv2.imshow("Video", img)
    if cv2.waitKey(1) == ord('q'):
        break
