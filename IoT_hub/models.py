from django.db import models

class WaterTankDepth(models.Model):
    depth = models.FloatField()
    date = models.DateTimeField("date published", auto_now_add=True)
    def __str__(self):
        return str(self.depth) + "cm at " + str(self.date)
