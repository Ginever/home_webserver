from django.db import models

class WaterTankDepth(models.Model):
    """
        A data class for data received from the water tank depth sensor
    """

    # The raw measured depth in cm
    #! look at implementing a low pass filter and normalizing time spread?
    depth = models.FloatField()

    # Time the data was received
    date = models.DateTimeField("date published", auto_now_add=True)
    def __str__(self):
        return str(self.depth) + "cm at " + str(self.date)
