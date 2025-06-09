CREATE DATABASE  IF NOT EXISTS `yourmusic` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;
USE `yourmusic`;
-- MySQL dump 10.13  Distrib 8.0.42, for Win64 (x86_64)
--
-- Host: localhost    Database: yourmusic
-- ------------------------------------------------------
-- Server version	8.0.42

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `album_authors`
--

DROP TABLE IF EXISTS `album_authors`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `album_authors` (
  `album_id` int NOT NULL,
  `usertag` varchar(45) NOT NULL,
  PRIMARY KEY (`album_id`,`usertag`),
  KEY `usertag` (`usertag`),
  CONSTRAINT `album_authors_ibfk_1` FOREIGN KEY (`album_id`) REFERENCES `albums` (`album_id`) ON DELETE CASCADE,
  CONSTRAINT `album_authors_ibfk_2` FOREIGN KEY (`usertag`) REFERENCES `users` (`usertag`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `album_authors`
--

LOCK TABLES `album_authors` WRITE;
/*!40000 ALTER TABLE `album_authors` DISABLE KEYS */;
INSERT INTO `album_authors` VALUES (20,'frank'),(2,'goshan'),(30,'harry'),(40,'jack'),(429,'justdice'),(430,'justdice'),(431,'justdice'),(432,'justdice'),(433,'justdice'),(434,'justdice'),(435,'justdice'),(436,'justdice'),(437,'justdice'),(438,'justdice'),(50,'leo'),(60,'nick'),(1,'stas');
/*!40000 ALTER TABLE `album_authors` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2025-06-09  0:48:52
